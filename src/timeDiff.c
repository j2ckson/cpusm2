#include "./timeDiff.h"

void time_diff(timeD xx, double rStat[], int em[], struct timespec exestart, struct timespec exeend)
{
	int roll = em[19];
	int rcnt = em[12];
	int longstart = em[26];
	char start_string[40];
	char trats_string[40];
	char end_string[40];
	char dne_string[40];
	double diff_time = ((double)exeend.tv_sec + (double)(1.0e-9*exeend.tv_nsec)) - ((double)exestart.tv_sec + (double)(1.0e-9*exestart.tv_nsec));
	if ( xx.sig == SIGINT || xx.sig == SIGSTOP) diff_time -= xx.secdecs;
	double pause_time = (double)xx.rate*(double)xx.slat*xx.secdecs*HSLEEP;
	if ( xx.samples != 0 ) xx.idur = xx.samples * xx.secdecs;
	int dhm[3] = {};
	double run_time = diff_time - pause_time;
	double a_samples = xx.rcnt;
	double rvar = ((rint(run_time/xx.secdecs) - a_samples) /rint(run_time/xx.secdecs));
	print_current_time_with_ms( start_string, trats_string, exestart);
	print_current_time_with_ms( end_string, dne_string, exeend);
	if (xx.sig == SIGINT || xx.sig == SIGQUIT || xx.sig == SIGSTOP ) {
		if ( xx.colour != 0 ) printf (CLOADLOW);
		double cpuspins = fmod(xx.cpuspin,60);
		int cpuspinm = (int)xx.cpuspin/60;
		int cpuspinh = (int)cpuspinm/60;
		double pcpu = xx.cpuspin / xx.cntsum * 100L / (double)xx.corecnt;
		printcc("\n\n start.time:\t\t  ", 2, xx.colour);
		printcc(start_string, 1, xx.colour);
		printcc(trats_string, 1, xx.colour);
		printcc("\n", 2, xx.colour);
		if (xx.sig == SIGQUIT || xx.sig == SIGSTOP) {
			printcc(" end.time:\t\t  ", 2, xx.colour);
			printcc(end_string, 1, xx.colour);
			printcc(dne_string, 1, xx.colour);
			printcc("\n", 2, xx.colour);
		}
		if (xx.sig == SIGINT && rcnt != 0 ) {
			printcc(" interim.time:\t\t  ", 2, xx.colour);
			printcc(end_string, 1, xx.colour);
			printcc(dne_string, 1, xx.colour);
			printcc("\n", 2, xx.colour);
		}
		double seconds = seconds_handler(dhm, run_time);
		if ( dhm[0] == 0 ) {
			printcc(" run.time:\t\t  ", 2, xx.colour);
			printcc("", 1, xx.colour);
			printf("%02i:%02i:%05.2lf\n", dhm[1],dhm[2],seconds);
		}
		if ( dhm[0] != 0 ) {
			printcc(" run.time:\t\t  ", 2, xx.colour);
			printcc("", 1, xx.colour);
			printf("%02id:%02i:%02i:%05.2lf\n", dhm[0],dhm[1],dhm[2],seconds);
		}
		if ( xx.sig == SIGSTOP || xx.sig == SIGINT ) {
			seconds = seconds_handler(dhm, diff_time+xx.secdecs);
		}else{
			seconds = seconds_handler(dhm, diff_time);
		}
		if ( dhm[0] == 0 ) {
			printcc(" active.time:\t\t  ", 2, xx.colour);
			printcc("", 1, xx.colour);
			printf("%02i:%02i:%05.2lf\n", dhm[1],dhm[2],seconds);
		}
		if ( dhm[0] != 0 ) {
			printcc(" active:\t\t  ", 2, xx.colour);
			printcc("", 1, xx.colour);
			printf("%02id:%02i:%02i:%05.2lf\n", dhm[0],dhm[1],dhm[2],seconds);
		}
		if ( pause_time != 0 ) {
			seconds = seconds_handler(dhm, pause_time);
			if ( dhm[0] == 0 ) {
				printcc(" defered.time:\t\t  ", 2, xx.colour);
				printcc("", 1, xx.colour);
				printf("%02i:%02i:%05.2lf\n", dhm[1],dhm[2],seconds);
			}
			if ( dhm[0] != 0 ) {
				printcc(" defered.time:\t\t  ", 2, xx.colour);
				printcc("", 1, xx.colour);
				printf("%02id:%02i:%02i:%05.2lf\n", dhm[0],dhm[1],dhm[2],seconds);
			}
		}
		printcc(" process.time:\t\t  ", 2, xx.colour);
		printcc("", 1, xx.colour);
		printf("%01i:%02i:%06.3lf\n", cpuspinh, cpuspinm, cpuspins);
		printcc(" \%cpu [core/cpu]:\t  ", 2, xx.colour);
		printcc("", 1, xx.colour);
		printf("%05.3lf/%05.3lf\n", pcpu*(double)xx.corecnt,pcpu);
		if ( u == 0 && ( xx.verb == 1 || xx.multistat == 1 ) ) {
			printcc(" rtt.packets.sent:\t  ", 2, xx.colour);
			printcc(commaprint(rStat[6]), 1, xx.colour);
			printcc("\n", 2, xx.colour);
			printcc(" rtt.packets.received:\t  ", 2, xx.colour);
			if ( rStat[7] != 0 ) printcc(commaprint(rStat[7]), 1, xx.colour);
			if ( rStat[7] == 0 ) printci(rStat[7], 1, xx.colour);
			printcc("\n", 2, xx.colour);		
			printcc(" rtt.packetloss:\t  ", 2, xx.colour);
			printcl(((1-rStat[7]/rStat[6])*100), 1, xx.colour);
			printcc("\%\n", 1, xx.colour);		
		}
		printcc(" expected.samples/s:\t  ", 2, xx.colour);
		printcc("", 1, xx.colour);
		printf("%-5.2lf\n", (double)1/(xx.secdecs));
		if (xx.sig == SIGQUIT || xx.sig == SIGSTOP) {
			printcc(" actual.samples/s:\t  ", 2, xx.colour);
			printcc("", 1, xx.colour);
			printf("%-5.2lf\n", ceil(a_samples/run_time));
		}
		if (xx.sig == SIGINT && rcnt!=0 ) {
			printcc(" interim.samples/s:\t  ", 2, xx.colour);
			printcc("", 1, xx.colour);
			printf("%-5.2lf\n", a_samples/run_time);
		}
		printcc(" expected.samples:\t  ", 2, xx.colour);
		printcc(commaprint(run_time/xx.secdecs), 1, xx.colour);
		printcc("\n", 2, xx.colour);
		if (xx.sig == SIGQUIT || xx.sig == SIGSTOP) {
			printcc(" actual.samples:\t  ", 2, xx.colour);
			printcc(commaprint(a_samples), 1, xx.colour);
			printcc("\n", 2, xx.colour);
		}
		if (xx.sig == SIGINT && rcnt!=0 ) {
			printcc(" interim.samples:\t  ", 2, xx.colour);
			printcc(commaprint(a_samples), 1, xx.colour);
			printcc("\n", 2, xx.colour);
		}
		printcc(" nominal.variation:\t  ", 2, xx.colour);
		printcc("", 1, xx.colour);
		printf("%-9.2lf\n", (a_samples - floor(run_time/xx.secdecs)));
		printcc(" relative.variation:\t  ", 2, xx.colour);
		printcc("", 1, xx.colour);
		printf("%-9.4lf\n\n\n", rvar);
		if ( xx.sig != SIGQUIT ) {
			return;
		}
	} else if ( ( xx.sig == SIGTRAP || xx.rcnt == 0 ) && xx.actlog <= 1 ) {
		if ( longstart == 0 || xx.rcnt != 0 ) {
			int rowgap = 0;
			if ( xx.dsize == 0 ) rowgap = 114;
			if ( xx.dsize == 1 ) rowgap = 83;
			printf(DEFCOLOR);
			printf ("\n");
			int rowwidth = 0;
			if ( xx.dsize == 0 ) rowwidth = 195;
			if ( xx.dsize == 1 ) rowwidth = 120;
			for ( int i=1;i<rowwidth;i++) {
				printcc("\u2500", 1, xx.colour);
			}
			printcc("\u2510\n", 1, xx.colour);
			int headerTitleSize = strlen(APPNAME);
			printcc(APPNAME, 4, xx.colour);
			for ( int i=headerTitleSize;i<rowwidth-1;i++ ) {
				printcc(" ", 4, xx.colour);
			}
			printf(DEFCOLOR);
			//printcc("", 0, xx.colour);
			printcc("\n", 0, 0);
			printf(HEADER);
			for ( int i=0;i<rowwidth-1;i++ ) {
				printcc(" ", 4, xx.colour);
			}
			//printcc("", 0, xx.colour);
			printf(DEFCOLOR);
			printcc("\n\n", 0, 0);
			printf(DEFCOLOR);
			printcc("\u250C\u2500", 1, xx.colour);
			printcc("SYSTEM.ATTRIBUTES", 2, xx.colour);
			for ( int i=0;i<40;i++) {
				printcc("\u2500", 1, xx.colour);
			}
			printcc("\u2510", 1, xx.colour);
			int rowlength = 61;
			for ( int i=0;i<rowgap-2-rowlength;i++ ) {
				printf(" ");
			}
			printcc("\u250C\u2500", 1, xx.colour);
			printcc("HEADING.KEY", 2, xx.colour);
			for ( int i=0;i<(rowlength-strlen("HEADING.KEY"))+20;i++) {
				printcc("\u2500", 1, xx.colour);
			}
			printcc("\u2510\n\n", 1, xx.colour);
			printcc("  system.type: ", 2, xx.colour);
			printcc(xx.sysid, 1, xx.colour);
			rowlength = strlen("  system.type: ") + strlen(xx.sysid);
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("", 2, xx.colour);
			printf("%05i: ", xx.ppid);
			printcc("application pid\n", 1, xx.colour);
			printcc("  hostname: ", 2, xx.colour);
			printcc(xx.uname, 1, xx.colour);
			rowlength = strlen("  hostname: ") + strlen(xx.uname);
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("a.freq: ", 2, xx.colour);
			printcc("average core frequency [MHz.per.interval]\n", 1, xx.colour);
			printcc("  o/s.version: ", 2, xx.colour);
			printcc(xx.version, 1, xx.colour);
			rowlength = strlen("  o/s.version: ") + strlen(xx.version);
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("a.\u00B0C: ", 2, xx.colour);
			printcc("average system temperature [per.interval]\n", 1, xx.colour);
			printcc("  cpu.model: ", 2, xx.colour);
			printcc(xx.model, 1, xx.colour);
			rowlength = strlen("  cpu.model: ") + strlen(xx.model);
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("free: ", 2, xx.colour);
			printcc("free system memory [\%]\n", 1, xx.colour);
			printcc("  cpu.cores: ", 2, xx.colour);
			char corecount[24];
			sprintf(corecount, "%d", xx.corecnt);
			printcc(corecount, 1, xx.colour);
			printcc(" [", 1, xx.colour);
			printcc(xx.cpuol, 1, xx.colour);
			printcc("]", 1, xx.colour);
			rowlength = strlen("  cpu.cores:  []") + strlen(corecount) + strlen(xx.cpuol);
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("avai: ", 2, xx.colour);
			printcc("available system memory [\%]\n", 1, xx.colour);
			char placeholder[11];
			sprintf(placeholder, "%.2f", xx.minfreq);
			printcc("  min.freq: ", 2, xx.colour);
			printcc(placeholder, 1, xx.colour);
			printcc("MHz" , 1, xx.colour);
			rowlength = strlen("  min.freq: MHz") + strlen(placeholder);
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("buff: ", 2, xx.colour);
			printcc("buffered system memory [\%]\n", 1, xx.colour);
			sprintf(placeholder, "%.2f", xx.maxfreq);
			printcc("  max.freq: ", 2, xx.colour);
			printcc(placeholder, 1, xx.colour);
			printcc("MHz" , 1, xx.colour);

			rowlength = strlen("  max.freq: MHz") + strlen(placeholder);
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("cach: ", 2, xx.colour);
			printcc("cached system memory [\%]\n", 1, xx.colour);
			printcc("  governor: ", 2, xx.colour);
			printcc(xx.freqgov, 1, xx.colour);
			rowlength = strlen("  governor: ") + strlen(xx.freqgov);
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("load.avg: ", 2, xx.colour);
			printcc("system load averages [per.interval]\n", 1, xx.colour);
			printcc("  sys.memory: ", 2, xx.colour);
			sprintf(placeholder, "%s", commaprint(xx.mmstat));
			printcc(placeholder, 1, xx.colour);
			printcc(" kB" , 1, xx.colour);
			rowlength = strlen("  sys.memory:  kB") + strlen(placeholder);
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("01mn: ", 2, xx.colour);
			printcc("one minute load average\n", 1, xx.colour);
			printcc("  sys.disks: ", 2, xx.colour);
			sprintf(placeholder, "%i ", xx.diskcnt);
			printcc(placeholder, 1, xx.colour);
			for (int i=0;i<xx.diskcnt;i++) {
				printcc("[", 1, xx.colour);
				printcc(xx.disk[i], 1, xx.colour);
				printcc("]", 1, xx.colour);
			}
			rowlength = strlen("  sys.disks: ") + strlen(placeholder);
			for (int i=0;i<xx.diskcnt;i++) {
				rowlength += strlen(xx.disk[i]);
				rowlength += strlen("[]");
			}
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("05mn: ", 2, xx.colour);
			printcc("five minute load average\n", 1, xx.colour);
			if ( xx.netdis != 0 ) printcc("  net.device: ", 2, xx.colour);
			printcc(xx.dev, 1, xx.colour);
			rowlength = strlen("  net.device: ") + strlen(xx.dev);
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("15mn: ", 2, xx.colour);
			printcc("fifteen minute load average\n", 1, xx.colour);
			printcc("  start.time: ", 2, xx.colour);
			printcc(start_string, 1, xx.colour);
			printcc(trats_string, 1, xx.colour);
			rowlength = strlen("  start time: ") + strlen(start_string) + strlen(trats_string);
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("procs: ", 2, xx.colour);
			printcc("system processes\n", 1, xx.colour);
			printcc("  current.time: ", 2, xx.colour);
			printcc(end_string, 1, xx.colour);
			printcc(dne_string, 1, xx.colour);
			rowlength = strlen("  current time: ") + strlen(end_string) + strlen(dne_string);
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("rnng: ", 2, xx.colour);
			printcc("running system processes\n", 1, xx.colour);
			double seconds = seconds_handler(dhm, xx.idur);
			if ( xx.rcnt != 0 ) {
				printcc("  interim.run.time: ", 2, xx.colour);
				printcc("", 1, xx.colour);
				if ( dhm[0] == 0 ) {
					printf("%02i:%02i:%05.2lf", dhm[1],dhm[2],seconds);
					rowlength = strlen("  interim.run.time: 00:00:00.00");
				}else {
					printf ("%02id:%02i:%02i:%05.2lf", dhm[0],dhm[1],dhm[2],seconds);
					rowlength = strlen("  interim.run.time: 00d:00:00:00.00");
				}
			}else{
				printcc("  expected.run.time: ", 2, xx.colour);
				printcc("", 1, xx.colour);
				if ( dhm[0] == 0 ) {
					printf("%02i:%02i:%05.2lf", dhm[1],dhm[2],seconds);
					rowlength = strlen("  expected.run.time: 00:00:00.00");
				}else {
					printf ("%02id:%02i:%02i:%05.2lf", dhm[0],dhm[1],dhm[2],seconds);
					rowlength = strlen("  expected.run.time: 00d:00:00:00.00");
				}
			}
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("actv: ", 2, xx.colour);
			printcc("active system processes [running+idle]\n", 1, xx.colour);
			printcc("  interval: ", 2, xx.colour);
			sprintf(placeholder, "%-7.3lf", xx.secdecs);
			printcc(placeholder, 1, xx.colour);
			rowlength = strlen("  interval: ") + strlen(placeholder);
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("cpu.avg: ", 2, xx.colour);
			printcc("cpu utilization averages [\%]\n", 1, xx.colour);
			double aoverrun = rcnt!=0?a_samples/run_time:0L;
			if ( rcnt != 0 ) {
				sprintf(placeholder, "%-5.2lf", aoverrun);
			}else{
				sprintf(placeholder, "%s", "n/a");
			}
			printcc("  interim.samples/s: ", 2, xx.colour);
			printcc(placeholder, 1, xx.colour);
			rowlength = strlen("  interim.samples/s: ") + strlen(placeholder);
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("spot: ", 2, xx.colour);
			printcc("per.interval average [cpu:\%][net:mbps][rtt:ms]\n", 1, xx.colour);
			printcc("  expected.samples/s: ", 2, xx.colour);
			sprintf(placeholder, "%-5.2lf", (double)1/(xx.secdecs));
			printcc(placeholder, 1, xx.colour);
			rowlength = strlen("  expected.samples/s: ") + strlen(placeholder);
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("runn: ", 2, xx.colour);
			printcc("running average [cpu:\%][net:mbps][rtt:ms]\n", 1, xx.colour);
			double asamples = rcnt!=0?a_samples:0L;
			if ( rcnt != 0 ) {
				sprintf(placeholder, "%s", commaprint(asamples));
			}else{
				sprintf(placeholder, "%s", "n/a");
			}
			printcc("  interim.samples: ", 2, xx.colour);
			printcc(placeholder, 1, xx.colour);
			rowlength = strlen("  interim.samples: ") + strlen(placeholder);
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("r.", 2, xx.colour);
			sprintf(placeholder, "%03i", roll);
			printcc(placeholder, 2, xx.colour);
			printcc(": ", 2, xx.colour);
			printcc("", 1, xx.colour);
			printf("%i", roll);
			printcc(".second rolling average [cpu:\%][net:mbps][rtt:ms]\n", 1, xx.colour);
			printcc("  expected.samples: ", 2, xx.colour);
			sprintf(placeholder, "%s", (xx.samples!=0?commaprint(xx.samples):((rint(xx.idur/(xx.secdecs)))!=0?commaprint(rint(xx.idur/(xx.secdecs))):"n/a")));
			printcc(placeholder, 1, xx.colour);
			rowlength = strlen("  expected.samples: ") + strlen(placeholder);
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("user: ", 2, xx.colour);
			printcc("normal.priority cpu utilization [\%]\n", 1, xx.colour);
			rowlength = 0;
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("nice: ", 2, xx.colour);
			printcc("low.priority cpu utilization [\%]\n", 1, xx.colour);
			printcc("\u250C\u2500", 1, xx.colour);
			printcc("RUNTIME.OPTIONS", 2, xx.colour);
			for ( int i=0;i<40;i++) {
				printcc("\u2500", 1, xx.colour);
			}
			printcc("\u2510", 1, xx.colour);
			rowlength = 61;
			for ( int i=0;i<rowgap+2-rowlength;i++ ) {
				printf(" ");
			}
			printcc("syst: ", 2, xx.colour);
			printcc("system.mode cpu utilization [\%]\n", 1, xx.colour);
			rowlength = 0;
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("idle: ", 2, xx.colour);
			printcc("idle.task cpu utilization [\%]\n", 1, xx.colour);
			printcc("  <h>: ", 2, xx.colour);
			printcc("show runtime help", 1, xx.colour);
			rowlength = strlen("  <h>: show runtime help");
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("iowa: ", 2, xx.colour);
			printcc("i/o.wait cpu utilization [\%]\n", 1, xx.colour);
			printcc("  <c>: ", 2, xx.colour);
			printcc("switch display colour characteristics", 1, xx.colour);
			rowlength = strlen("  <c>: switch display colour characteristics");
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("irqs: ", 2, xx.colour);
			printcc("interrupt cpu utilization [\%]\n", 1, xx.colour);
			printcc("  <q>: ", 2, xx.colour);
			printcc("quit cpusm", 1, xx.colour);
			rowlength = strlen("  <q>: quit cpusm");
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("sirq: ", 2, xx.colour);
			printcc("soft.irq cpu utilization [\%]\n", 1, xx.colour);
			printcc("  <s>: ", 2, xx.colour);
			printcc("display interim cpusm summary statistics", 1, xx.colour);
			rowlength = strlen("  <s>: display interim cpusm summary statistics");
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("maxi: ", 2, xx.colour);
			printcc("maximum 'spot' cpu utilization [\%]\n", 1, xx.colour);
			printcc("  <x>: ", 2, xx.colour);
			printcc("enable/disable periodic header printing", 1, xx.colour);
			rowlength = strlen("  <x>: enable/disable periodic header printing");
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("count: ", 2, xx.colour);
			printcc("total samples processed\n", 1, xx.colour);
			printcc("  <j>: ", 2, xx.colour);
			printcc("enable/disable step display", 1, xx.colour);
			rowlength = strlen("  <j>: enable/disable step display");
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("algn: ", 2, xx.colour);
			printcc("alignment of processing & acquisition threads [\%]\n", 1, xx.colour);
			printcc("  <m>: ", 2, xx.colour);
			printcc("shift field focus", 1, xx.colour);
			rowlength = strlen("  <m>: shift field focus");
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("pcpu: ", 2, xx.colour);
			printcc("cpu utilization by the cpusm process [\%]\n", 1, xx.colour);
			printcc("  <z>: ", 2, xx.colour);
			printcc("restart cpusm", 1, xx.colour);
			rowlength = strlen("  <z>: restart cpusm");
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("runtime: ", 2, xx.colour);
			printcc("cumulative cpusm running time\n", 1, xx.colour);
			printcc("  </>: ", 2, xx.colour);
			printcc("switch output mode [fixed-line/scrolling]", 1, xx.colour);
			rowlength = strlen("  </>: switch output mode [fixed-line/scrolling]");
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("schd: ", 2, xx.colour);
			printcc("kernel scheduling algorithm\n", 1, xx.colour);
			printcc("  <.>: ", 2, xx.colour);
			printcc("display column headings", 1, xx.colour);
			rowlength = strlen("  <.>: display column headings");
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("pri: ", 2, xx.colour);
			printcc("kernel scheduling priority\n", 1, xx.colour);
			printcc("  <=>: ", 2, xx.colour);
			printcc("display interim min/max statistics", 1, xx.colour);
			rowlength = strlen("  <=>: display interim min/max statistics");
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("nce: ", 2, xx.colour);
			printcc("application priority ['nice']\n", 1, xx.colour);
			printcc("  <l>: ", 2, xx.colour);
			printcc("enable/disable shifting field focus", 1, xx.colour);
			rowlength = strlen("  <l>: enable/disable shifting field focus");
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("core: ", 2, xx.colour);
			printcc("virtual core affinity\n", 1, xx.colour);
			printcc("  <v>: ", 2, xx.colour);
			printcc("switch statistics display length]", 1, xx.colour);
			rowlength = strlen("  <v>: switch statistics display length]");
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("rtt.avg: ", 2, xx.colour);
			printcc("round.trip.time averages[ms per.interval]\n", 1, xx.colour);
			printcc("  <`>: ", 2, xx.colour);
			printcc("switch pcpu accounting [per-core/aggregate]", 1, xx.colour);
			rowlength = strlen("  <`>: switch pcpu accounting [per-core/aggregate]");
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("rtt.trk: ", 2, xx.colour);
			printcc("rtt tracking [tx/s:tx/sec][rx/s:rx/sec][\%loss:packet loss \%\n", 1, xx.colour);
			printcc("  <1>: ", 2, xx.colour);
			printcc("switch runtime precision [on/off]", 1, xx.colour);
			rowlength = strlen("  <1>: switch runtime precision [on/off]");
			for ( int i=rowlength;i<rowgap-1;i++ ) {
				printf(" ");
			}
			printcc("pp/s: ", 2, xx.colour);
			printcc("packets per second\n", 1, xx.colour);
			if ( xx.cpun != -9 ) {
				printcc("  <a>: ", 2, xx.colour);
				printcc("switch primary thread core affinity\n", 1, xx.colour);
			}
			if ( xx.actlog >= 1 ) {
				printcc("  <,>: ", 2, xx.colour);
				printcc("enable/diable stdout [file logging-mode]]\n", 1, xx.colour);
			}
			if ( xx.multistat == 1 ) {
				printcc("  < >: ", 2, xx.colour);
				printcc("switch statistics mode\n", 1, xx.colour);
			}
			if ( xx.samples != 0 || xx.idurv != 0 ) {
				printcc("  <n>: ", 2, xx.colour);
				printcc("switch 'runtime' statistic [running-time*, remaining-time]\n", 1, xx.colour);
				printcc("  <b>: ", 2, xx.colour);
				printcc("switch 'count' statistic [running-samples*, remaining-samples]\n", 1, xx.colour);
			}
			printf("\n");
			printcc(xx.appstart, 3, xx.colour);
			printf("\n\n");
			if ( xx.sig == SIGTRAP && xx.scroll != 0 ) {
				for (int i=HSLEEP; i>0; i--) {
					usleep(xx.rate*xx.secdecs*USECS);
					printf("  active monitoring resumes in %02i seconds\n", i-1);
					printf ("\x1b[1A");
				}
			}
		}
		printf("\n");
	}
	return;
}
