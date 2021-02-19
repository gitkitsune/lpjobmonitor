#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int lptest();
int run_time();
char* c_time_string;

int main(void)
{
  printf("\nSimple program to switch on/off printer at jobs in queue.\nThis is total for free. If you like it, write to <jaroslaw.strauchmann@gmail.com>\n\n");
  char *usb_on;
  char *com_on;
  char *chmod;
  char *com_off;
  char *usb_off;
  char *ctj;
  char *ctt;
  char *ctl;
  char *slp;
  size_t comsize = 100;
  size_t timesize = 20;

  FILE * config;
  config = fopen("/etc/lpjobmonitor-config", "r");
  if (config == NULL) {
	printf("There is no config file. Config file must be stored on /etc/lpjobmonitor-config\n\n\nConfig file must have 9 lines.\nFirst line: command executed to tun on USB\nSecond line: command executed to turn on printer.\nThrird line: command executed to change USB permission.\nFourth line: command executed to turn off printer.\nFifth line: command executed to turn off USB.\nIn next four lines specify time in seconds for:\nWait to next job.\nTime to shut down printer after print all jobs.\nMain loop time.\nTime between system commands.\n");
	printf("In my case, the configuration file looks like this:\n\nmodprobe xhci_mtk\necho default-on > /sys/class/leds/r6220\\:green\\:wps/trigger\nchmod 0666 /dev/bus/usb/001/*\necho none > /sys/class/leds/r6220\\:green\\:wps/trigger\nrmmod xhci_mtk\n30\n150\n15\n1\n\nIf you do not need to execute so many commands (eg, change permissions on the filesystem, or disable and enable USB), then leave these lines blank, terminated with a UNIX-style line break.\n\n");
	exit(EXIT_FAILURE);
	}

  usb_on = (char *)malloc(comsize * sizeof(char));
  com_on = (char *)malloc(comsize * sizeof(char));
  chmod = (char *)malloc(comsize * sizeof(char));
  com_off = (char *)malloc(comsize * sizeof(char));
  usb_off = (char *)malloc(comsize * sizeof(char));
  ctj = (char *)malloc(timesize * sizeof(char));
  ctt = (char *)malloc(timesize * sizeof(char));
  ctl = (char *)malloc(timesize * sizeof(char));
  slp = (char *)malloc(timesize * sizeof(char));
  getline(&usb_on,&comsize,config);
  getline(&com_on,&comsize,config);
  getline(&chmod,&comsize,config);
  getline(&com_off,&comsize,config);
  getline(&usb_off,&comsize,config);
  getline(&ctj,&timesize,config);
  getline(&ctt,&timesize,config);
  getline(&ctl,&timesize,config);
  getline(&slp,&comsize,config);

  fclose(config);

  printf("Check below parameters from config file:\n\nTurn on command: %sTurn off command: %sTurn on USB: %sTurn off USB: %sUSB chmod command: %sWait to next job: %sWait to turn off: %sMain loop time: %sTime to wait after turn on USB: %s\n",com_on, com_off, usb_on, usb_off, chmod, ctj, ctt, ctl, slp);

	int q = 0;

	int tj, tt, tl, sl;
	sscanf(ctj, "%d", &tj);
	sscanf(ctt, "%d", &tt);
	sscanf(ctl, "%d", &tl);
	sscanf(slp, "%d", &sl);

/* work in loop */

while (q != EOF) {
	q = lptest();
	if (q != 0) {
		run_time();
		printf("A print start command was invoked %s", c_time_string); /*system command to enable printer - taken from config - line 3*/
		system(usb_on);
		sleep(sl);
		system(com_on);
		sleep(sl);
		system(chmod);
		while ( q !=0 ) {
			if (lptest() != 0) {
			/*printf("Trwa drukowanie\n"); tmp for debugging, checking the loop*/
			sleep(tj); /*wait tj time before check for next jobs*/
			}
			else {
			printf("It seems that nothing is being printed anymore. If there are no jobs for the specified time, it turns off the printer.\n"); /*tmp for debugging, checking the loop*/
			sleep(tt); /*wait before shut down the printer*/
			q = lptest();
			}
		}
		run_time();
		printf("The print exit command was invoked %s", c_time_string); /*system command to turn off the printer - taken from config - line 4*/
		system(com_off);
		sleep(sl);
		system(usb_off);
	}
	else
/*	printf ("The buffer value is: %d\n", q); */
	sleep(tl); /*Loop check time for jobs*/
	}
}

/* check CUPS jobs to do */

int lptest()
{
	int q;
	FILE *fp = popen("lpstat -o | wc -l", "r");
	fscanf(fp, "%d", &q);
	pclose(fp);
	return q;
}

int run_time()
{
	time_t current_time;
	current_time = time(NULL);
	c_time_string = ctime(&current_time);
}
