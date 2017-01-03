/*
 * main.c -- Main program for the GoAhead WebServer (LINUX version)
 *
 * Copyright (c) GoAhead Software Inc., 1995-2010. All Rights Reserved.
 *
 * See the file "license.txt" for usage and redistribution license requirements
 *
 */

/******************************** Description *********************************/

/*
 *	Main program for for the GoAhead WebServer.
 */

/********************************* Includes ***********************************/
#include <stdio.h>  
#include <stdlib.h>  
#include <sys/wait.h>
#include	"../uemf.h"
#include	"../wsIntrn.h"
#include	<signal.h>
#include	<unistd.h>
#include	<sys/types.h>
#include <sys/reboot.h>

#include <public.h>
#include <sysinfo.h>

#ifdef WEBS_SSL_SUPPORT
#include	"../websSSL.h"
#endif

#define USER_MANAGEMENT_SUPPORT 1
#ifdef USER_MANAGEMENT_SUPPORT
#include	"../um.h"
void	formDefineUserMgmt(void);
#endif


/*********************************** Locals ***********************************/
/*
 *	Change configuration here
 */

static char_t		*rootWeb = T("www");			/* Root web directory */
static char_t		*demoWeb = T("wwwdemo");		/* Root web directory */
static char_t		*password = T("");				/* Security password */
static int			port = WEBS_DEFAULT_PORT;		/* Server port */
static int			retries = 5;					/* Server port retries */
static int			finished = 0;					/* Finished flag */

/****************************** Forward Declarations **************************/

static int 	initWebs(int demo);
//static int	aspTest(int eid, webs_t wp, int argc, char_t **argv);
//static void formTest(webs_t wp, char_t *path, char_t *query);
//extern int formUnit(webs_t wp, char_t *url, char_t *path, char_t *query);
static void formUpload(webs_t wp, char_t *path, char_t *query); // add by gyr 2011.10.15
static int  websHomePageHandler(webs_t wp, char_t *urlPrefix, char_t *webDir,
				int arg, char_t *url, char_t *path, char_t *query);
extern int aspChassisView(int eid, webs_t wp, int argc, char_t **argv);
//extern int aspInclude(int eid, webs_t wp, int argc, char_t **argv);
extern int aspNmuStatus(int eid, webs_t wp, int argc, char_t **argv);
extern int aspUnitList(int eid, webs_t wp, int argc, char_t **argv);
extern void unitAspDefine();
extern void alarmAspDefine();
extern void sysAspDefine();
int websAjaxHandler(webs_t wp, char_t *urlPrefix, char_t *webDir, int arg,   
char_t *url, char_t *path, char_t *query);
char show_alarm_count=1;

static void	sigintHandler(int);
#ifdef B_STATS
static void printMemStats(int handle, char_t *fmt, ...);
static void memLeaks();
#endif

/*********************************** Code *************************************/
/*
 *	Main -- entry point from LINUX
 */
static void usage()
{
  printf("Description:\n");
  printf("web server\nLast modified:%s\n\n",LAST_MODIFY_DATE);
  printf("Usage:webs[option] \n");
  printf("Option Specification:\n");
  printf("-d: set as debug mode\n");
  /*
  printf("-c: clear olp optical switch conf\n");
  printf("-r: reboot ccenter\n");
  printf("-s: slot of unit\n");
  */
  printf("-h for help.\n");
}
extern int rows_per_page;
static void init_conf(int argc, char **argv)
{
   char value[16]="";
   char save_alarm_flag;
   char show_acount;

   getConfValue(SYS_CONF_FILE,"save_alarms",value," ");
   if (value[0]=='2')
      save_alarm_flag=2;
   else if (value[0]=='0')
      save_alarm_flag=0;
   else
      save_alarm_flag=1;
   getConfValue(SYS_CONF_FILE,"show_acount",value," ");
   if (value[0]!='N')
       show_acount=1;
   else
       show_acount=0;
   if (show_acount==1 && (save_alarm_flag==0 ||  save_alarm_flag==1))
      show_alarm_count=1;
   else
      show_alarm_count=2;
   rows_per_page=getConfIntValue(SYS_CONF_FILE,"maxrows"," ");
   if (rows_per_page<20)
      rows_per_page=20;
   initAlarmConf();
   initDebug();
  int oc;
  //printf("Auto reboot testing...0\r\n");
  while((oc = getopt(argc, argv, "dh")) != -1)
  {
     //printf("oc=%d\r\n",oc);
     switch(oc)
     {
       case 'd':
         debug.is_debug_mode=1;
       break;
       case 'h':
         usage();
       exit(0);
     }
  }
}
int main(int argc, char** argv)
{
	
         int i, demo = 0;

	 for (i = 1; i < argc; i++) {
           if (strcmp(argv[i], "-v") == 0)
           {
             printf("Web server version:%s\n",WEBS_VERSION);
             exit(0);
           }
	   if (strcmp(argv[i], "-demo") == 0) {
		demo++;
	   }
	}
         
         init_conf(argc,argv);
         if (lockFile("/vispace/webs.lck")<0)
         {
          exit(0);
         }
         
/*
 *	Initialize the memory allocator. Allow use of malloc and start
 *	with a 60K heap.  For each page request approx 8KB is allocated.
 *	60KB allows for several concurrent page requests.  If more space
 *	is required, malloc will be used for the overflow.
 */
	bopen(NULL, (60 * 1024), B_USE_MALLOC);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, sigintHandler);
	signal(SIGTERM, sigintHandler);

/*
 *	Initialize the web server
 */
	if (initWebs(demo) < 0) {
		printf("initWebs error.\n");	// added by gyr 2011.09.17
		return -1;
	}

#ifdef WEBS_SSL_SUPPORT
	websSSLOpen();
/*	websRequireSSL("/"); */	/* Require all files be served via https */
#endif

/*
 *	Basic event loop. SocketReady returns true when a socket is ready for
 *	service. SocketSelect will block until an event occurs. SocketProcess
 *	will actually do the servicing.
 */
        //init();
	finished = 0;
	while (!finished) {
		if (socketReady(-1) || socketSelect(-1, 1000)) {
			socketProcess(-1);
		}
		websCgiCleanup();
		emfSchedProcess();
	}

#ifdef WEBS_SSL_SUPPORT
	websSSLClose();
#endif

#ifdef USER_MANAGEMENT_SUPPORT
	umClose();
#endif

/*
 *	Close the socket module, report memory leaks and close the memory allocator
 */
	websCloseServer();
	socketClose();
#ifdef B_STATS
	memLeaks();
#endif
	bclose();
	return 0;
}

/*
 *	Exit cleanly on interrupt
 */
static void sigintHandler(int unused)
{
	finished = 1;
}

/*uchar_t getChassisClass()
{
 if (chassis_class<1 || chassis_class>4)
 {
   chassis_class=getUnitClass(0,0);
 }
 return chassis_class;
}
*/
static int retCode=0;

int getRetCode()
{
  return retCode;
}
int aspUserLevel(int eid, webs_t wp, int argc, char_t **argv)
{
  short privilege=umGetUserPrivilege(wp->userName);
  if (privilege<2)
     privilege=2;
  else
     privilege=0;
  return websWrite(wp,T("%d"),privilege);
}

int aspInclude(int eid, webs_t wp, int argc, char_t **argv)
{
 int bytes=0;
 uchar_t chassis_class;
 chassis_class=getChassisClass(0);
 /*
 if (chassis_class<1 || chassis_class>4)
 {
   //chassis_class=getUnitClass(0,0);
   chassis_class=getChassisClass(0);
 }
 */
 if (chassis_class!=4)
 {
   char view[16];
   bytes=websWrite(wp, "\n<link href=\"css/");
   if (chassis_class!=2)
     strcpy(view,"not4uview");
   else
      strcpy(view,"twouview");
     bytes=websWrite(wp, T("%s.css"),view);
   bytes=websWrite(wp, "\" rel=\"stylesheet\" type=\"text/css\">");
  bytes=websWrite(wp, "\n<!--[if lte IE 8]>\n\
<style type=\"text/css\">\n\
.sl,.ml{\n\
   -ms-filter: \"progid:DXImageTransform.Microsoft.BasicImage(rotation=1)\";\n\
   filter: progid:DXImageTransform.Microsoft.BasicImage(rotation=1);\n\
}\n\
.leds .sl{height:16px;width:26px;font-size:10px;}\n\
</style>\n\
<![endif]-->\n");
  bytes+=websWrite(wp, T("\n<script src=\"js/%s.js\" type=\"text/javascript\"></script>\n"),view);
 }
 else
   bytes+=websWrite(wp, "<link href=\"css/cview.css\" rel=\"stylesheet\" type=\"text/css\">");
  return bytes;
}
/******************************************************************************/
/*
 *	Initialize the web server.
 */

static int initWebs(int demo)
{
	struct hostent	*hp;
	struct in_addr	intaddr;
	char			host[128], dir[128], webdir[128];
	char			*cp;
	char_t			wbuf[128];

/*
 *	Initialize the socket subsystem
 */
	socketOpen();

#ifdef USER_MANAGEMENT_SUPPORT
/*
 *	Initialize the User Management database
 */
	umOpen();
	umRestore(T("umconfig.txt"));
#endif

/*
 *	Define the local Ip address, host name, default home page and the
 *	root web directory.
 */
 /* del by gyr 2011.09.17
	if (gethostname(host, sizeof(host)) < 0) {
		error(E_L, E_LOG, T("Can't get hostname"));
		printf("initWebs::Can't get hostname.\n");		// added by gyr 2011.09.17
		return -1;
	}
	if ((hp = gethostbyname(host)) == NULL) {
		error(E_L, E_LOG, T("Can't get host address"));
		printf("initWebs::Can't get hostname...\n");	// added by gyr 2011.09.17
		return -1;
	}
	memcpy((char *) &intaddr, (char *) hp->h_addr_list[0],
		(size_t) hp->h_length);
*/
        if (!getConfValue("/etc/network/interfaces","address",host," "))
          intaddr.s_addr = inet_addr(T(host));
        else
	  intaddr.s_addr = inet_addr(T("192.168.1.168"));		// added by gyr 2011.09.17

/*
 *	Set ../web as the root web. Modify this to suit your needs
 *	A "-demo" option to the command line will set a webdemo root
 */
	getcwd(dir, sizeof(dir));// Get the name of the current working directory, note | gyr 2011.09.17
	if ((cp = strrchr(dir, '/'))) {// \B2\E9\D5\D2\D7ַ\FB\D4\DAָ\B6\A8\D7ַ\FB\B4\AE\D6дӺ\F3\C3濪ʼ\B5ĵ\DAһ\B4γ\F6\CFֵ\C4λ\D6ã\AC
		*cp = '\0';					// \C8\E7\B9\FB\B3ɹ\A6\A3\AC\D4򷵻\D8ָ\CF\F2\B8\C3λ\D6õ\C4ָ\D5룬\C8\E7\B9\FBʧ\B0ܣ\AC\D4򷵻\D8 false\A1\A3
	}
	if (demo) {
		sprintf(webdir, "%s/%s", dir, demoWeb);
	} else {
		sprintf(webdir, "%s/%s", dir, rootWeb);
	}

/*
 *	Configure the web server options before opening the web server
 */
	websSetDefaultDir(webdir);
	cp = inet_ntoa(intaddr);
	ascToUni(wbuf, cp, min(strlen(cp) + 1, sizeof(wbuf)));
	websSetIpaddr(wbuf);
	ascToUni(wbuf, host, min(strlen(host) + 1, sizeof(wbuf)));
	websSetHost(wbuf);

/*
 *	Configure the web server options before opening the web server
 */
	websSetDefaultPage(T("index.htm"));
	websSetPassword(password);

/*
 *	Open the web server on the given port. If that port is taken, try
 *	the next sequential port for up to "retries" attempts.
 */
	websOpenServer(port, retries);

/*
 * 	First create the URL handlers. Note: handlers are called in sorted order
 *	with the longest path handler examined first. Here we define the security
 *	handler, forms handler and the default web page handler.
 */
	websUrlHandlerDefine(T(""), NULL, 0, websSecurityHandler,
		WEBS_HANDLER_FIRST);
	websUrlHandlerDefine(T("/goform"), NULL, 0, websFormHandler, 0);
	websUrlHandlerDefine(T("/cgi-bin"), NULL, 0, websCgiHandler, 0);
	websUrlHandlerDefine(T(""), NULL, 0, websDefaultHandler,WEBS_HANDLER_LAST);
        //websUrlHandlerDefine(T("/unit"), NULL, 0, formUnit, WEBS_HANDLER_FIRST);

/*
 *	Now define two test procedures. Replace these with your application
 *	relevant ASP script procedures and form functions.
 */
	//websAspDefine(T("aspTest"), aspTest);
	//websFormDefine(T("formTest"), formTest);
      /*websFormDefine(T("formUpload"), formUpload);// add by gyr 2011.10.15
 
      websAspDefine(T("ChassisView"), aspChassisView);
      websAspDefine(T("Include"), aspInclude);
      websAspDefine(T("NmuStatus"), aspNmuStatus);
      websAspDefine(T("unitList"), aspUnitList);
      websAspDefine(T("UserLevel"), aspUserLevel);
      
      unitAspDefine();
      alarmAspDefine();
      sysAspDefine();
      */
      toolAspDefine();
      websUrlHandlerDefine(T("/ajax"), NULL, 0, websAjaxHandler, 0);

/*
 *	Create the Form handlers for the User Management pages
 */
#ifdef USER_MANAGEMENT_SUPPORT
	formDefineUserMgmt();
#endif

/*
 *	Create a handler for the default home page
 */
	websUrlHandlerDefine(T("/"), NULL, 0, websHomePageHandler, 0);

	return 0;
}

/******************************************************************************/
/*
 *	Test Javascript binding for ASP. This will be invoked when "aspTest" is
 *	embedded in an ASP page. See web/asp.asp for usage. Set browser to
 *	"localhost/asp.asp" to test.
 */
/*
static int aspTest(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t	*name, *address;

	if (ejArgs(argc, argv, T("%s %s"), &name, &address) < 2) {
		websError(wp, 400, T("Insufficient args\n"));
		return -1;
	}
	return websWrite(wp, T("Name: %s, Address %s"), name, address);
}
*/
/******************************************************************************/
/*
 *	Test form for posted data (in-memory CGI). This will be called when the
 *	form in web/forms.asp is invoked. Set browser to "localhost/forms.asp" to test.
 */

static void formTest(webs_t wp, char_t *path, char_t *query)
{
	char_t	*name, *address;

	name = websGetVar(wp, T("name"), T("Joe Smith"));
	address = websGetVar(wp, T("address"), T("1212 Milky Way Ave."));

	websHeader(wp);
	websWrite(wp, T("<body><h2>Name: %s, Address: %s</h2>\n"), name, address);
	websFooter(wp);
	websDone(wp, 200);
}

/******************************************************************************/
/*
 *	Home page handler
 */

static int websHomePageHandler(webs_t wp, char_t *urlPrefix, char_t *webDir,
	int arg, char_t *url, char_t *path, char_t *query)
{
/*
 *	If the empty or "/" URL is invoked, redirect default URLs to the home page
 */
        if (*url == '\0' || gstrcmp(url, T("/")) == 0) {
                //printf("WEBS_DEFAULT_HOME:%s\n",WEBS_DEFAULT_HOME);
		websRedirect(wp, WEBS_DEFAULT_HOME);
		return 1;
	}
        /*
        #ifdef DEFAULT_LANG_CN
	if (*url == '\0' || gstrcmp(url, T("/")) == 0) {
                //printf("WEBS_DEFAULT_HOME:%s\n",WEBS_DEFAULT_HOME);
		websRedirect(wp, WEBS_DEFAULT_HOME);
		return 1;
	}
        #else
          if (*url == '\0' || gstrcmp(url, T("/")) == 0) {
                //printf("WEBS_DEFAULT_HOME:%s\n",WEBS_DEFAULT_HOME);
		websRedirect(wp, WEBS_DEFAULT_HOME);
		return 1;
	  }
          else if (!gstrncmp(url, T("/en/"),4))
          {
              char buf[128];
              sprintf(buf,"");
              printf("url=%s\n",url);
              sprintf(buf,"/en/",url);
              websRedirect(wp, buf);
          }
        #endif
        */
	return 0;
}

/******************************************************************************/

#ifdef B_STATS
static void memLeaks()
{
	int		fd;

	if ((fd = gopen(T("leak.txt"), O_CREAT | O_TRUNC | O_WRONLY, 0666)) >= 0) {
		bstats(fd, printMemStats);
		close(fd);
	}
}

/******************************************************************************/
/*
 *	Print memory usage / leaks
 */

static void printMemStats(int handle, char_t *fmt, ...)
{
	va_list		args;
	char_t		buf[256];

	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);
	write(handle, buf, strlen(buf));
}
#endif

int test=0;
int websAjaxHandler(webs_t wp, char_t *urlPrefix, char_t *webDir, int arg,   
char_t *url, char_t *path, char_t *query)  
{  
  // #define WEBS_CACHE_CONTROL_STRING    T("Cache-Control: no-store, no-cache, must-revalidate, post-check=0, pre-check=0\r\nPragma: no-cache\r\n")
    
    websHeader(wp);
    websWrite(wp, T("<h2>this is ajax test:%d!</h2>\n"),test++);
    websFooter(wp);
    websDone(wp, 200);
    return 1;
/*
    websHeader(wp);  
    websWrite(wp,"<script type=\"text/javascript\">\
function myrefresh()\
{\
  window.location.href=\"/ajax.htm\";\
}\n \
setTimeout(myrefresh,2000);\
</script>");
    websWrite(wp, T("<h2>this is ajax test:%d!</h2>\n"),test);
    test++;
    websFooter(wp);
    websDone(wp, 200); 
    return 1;
*/
}  

/******************************************************************************/
/*
 * for test html upload file to web server
 * add by gyr 2011.10.15
 */

static void formUpload(webs_t wp, char_t *path, char_t *query)
{
    #define UNKOWN_FILE 0
    #define CCENTER_FILE 1
    #define SNMP_TAR_FILE 2
    #define WEBS_TAR_FILE 3
    #define CLI_TAR_FILE 4
    #define NMU_TAR_FILE 5
    #define SNMPD_FILE 6
    #define CLI_FILE 7
    #define WEBS_FILE 8
    #define IMAGE_FILE 9
    #define DATABASE_FILE 10
    #define UNIT_FILE 30

    #define UNKOWN_FILE_TYPE 0
    #define TAR_FILE_TYPE 1
    #define BIN_FILE_TYPE 2
    #define IMAGE_FILE_TYPE 3

    #define SNMP_BIN_FILE 1
    #define CLI_BIN_FILE 2
    #define WEB_BIN_FILE 3
    #define CCENTER_BIN_FILE 4

    FILE *       fp;
    char_t *     fn;
    char_t *     bn = NULL;
    int          locWrite;
    int          numLeft;
    int          numWrite=0;
    char         savetopath[36]="";
    char file_flag=UNKOWN_FILE_TYPE;
    
    char file_type=0;
    char is_bin_file=0;
    /*
    pid_t pid;
    pid=getPidByName("pidof heartbeat");
    doSystem("kill -9 %ld",pid);
    */
    printf("\n...................Uploading...................\n\n");
    //system("killall -9 heartbeat");
    //sleep(3);
    
    //system("killall -9 alarmd");
    a_assert(websValid(wp));
    websHeader(wp);
    /*
    pid=getPidByName("pidof watchdogd");
    if (pid>0)
    {
       websWrite(wp, T("<div style='background-color:#CCC0C0;'>watchdogd %ld</div>"),pid);
       websFooter(wp);
       printf("reboot2...");
       websDone(wp, 200);
       return;
    }
    */
    fn = websGetVar(wp, T("filename"), T(""));
    if (fn != NULL && *fn != '\0') {
        if ((int)(bn = gstrrchr(fn, '/') + 1) == 1) {
            if ((int)(bn = gstrrchr(fn, '\\') + 1) == 1) {
                bn = fn;
            }
        }
    }
    //system("killall -9 heartbeat");
    printf("file name:%s, bn=%s  .......\n", fn, bn);
    websWrite(wp, T("<div style='background-color:#CCC0C0;'>"));
    if (bn)
    {
      if (strendwith(bn,".tar.gz") ||strendwith(bn,".tar"))
      {
       file_type=TAR_FILE_TYPE;
       system("rm /*.tar.gz");
       //system("killall -9 watchdogd");
       //printf("killall -9 watchdogd\n");
       strcpy(savetopath,"");
       if (strstr(bn,"snmp"))
       {
         file_flag=SNMP_TAR_FILE;
         //system("rm /bin/snmpd");
       }
       else if (strstr(bn,"webs"))
       {
          file_flag=WEBS_TAR_FILE;
          //system("rm /vispace/webs/webs");
          //strcpy(savetopath,"/vispace/");
       }
       else if (strstr(bn,"cli"))
       {
          file_flag=CLI_TAR_FILE;
          //system("rm /bin/clish");
       }
       else if (strstr(bn,"nmu"))
       {
          file_flag=NMU_TAR_FILE;
          //system("rm /bin/ccenter");
          //system("rm /bin/clish");
          //system("rm /bin/snmpd");
          //system("rm /vispace/webs/webs");
       }
      }
      else if (strendwith(bn,".bin") && (strstr(bn,"olp") || strstr(bn,"obp") || strstr(bn,"otu") || strstr(bn,"oeo") || strstr(bn,"edfa") || strstr(bn,"otap")))
      {
          file_flag=UNIT_FILE;
          strcpy(savetopath,"/tmp");
      }
      else if (strendwith(bn,".jpg") || strendwith(bn,".gif") ||strendwith(bn,".png"))
      {
       file_flag=IMAGE_FILE;
       char logo[16]="logo3.";
       if (strendwith(bn,".jpg"))
           strcat(logo,"jpg");
       else if (strendwith(bn,".gif"))
            strcat(logo,"gif");
       else 
            strcat(logo,"png");
       strcpy(savetopath,"/vispace/www/images");
       //setConfValue("/vispace/nmu.bi","logo",bn,"logo.jpg");
       setVendorInfo(VENDOR_LOGO,logo);
      }
      else if (strendwith(bn,".db"))
      {
          file_flag=DATABASE_FILE;
          strcpy(savetopath,"/vispace/www/data");
      }
      else 
      {
        strcpy(savetopath,"/bin");
        is_bin_file=1;
        if (strstr(bn,"webs"))
        {
          file_flag=WEBS_FILE;
          strcpy(savetopath,"/vispace/webs");
        }
        else if (strstr(bn,"snmpd"))
          file_flag=SNMPD_FILE;
        else if (strstr(bn,"clish"))
          file_flag=CLI_FILE;
        else if (strstr(bn,"ccenter"))
          file_flag=CCENTER_FILE;
        else
            is_bin_file=0;
        if (is_bin_file!=0)
         doSystem("rm %s/%s",savetopath,fn);
      }
    }
    #ifdef DEFAULT_LANG_CN
         websWrite(wp, T("文件名 = %s<br>大小 = %d 字节<br>"), bn, wp->lenPostData);
    #else
         websWrite(wp, T("File name = %s<br>Size = %d bytes<br>"), bn, wp->lenPostData);
    #endif
    
    if (bn==NULL ||UNKOWN_FILE==file_flag)
    {
       websWrite(wp, T("Invalid file</div>"));
       return;
    }
/*
    if (SNMPD_FILE==file_flag || CLI_FILE==file_flag|| CCENTER_FILE==file_flag)
    {
       //printf("	This is webs\n");
       //doSystem("rm /bin/%s",bn);
       doSystem("killall -9 %s",bn);
    }
    if (CLI_FILE==file_flag)
    {
       system("rm /bin/clish");
    }
    else if (WEBS_FILE==file_flag)
    {
       //printf("	This is webs\n");
       system("rm /vispace/webs/webs");
       strcpy(savetopath,"/vispace/webs");
    }
    else if (SNMPD_FILE==file_flag || CLI_FILE==file_flag|| CCENTER_FILE==file_flag)
    {
       //printf("	This is webs\n");
       doSystem("rm /bin/%s",bn);
       doSystem("killall -9 %s",bn);
       //strcpy(savetopath,"/vispace/webs/");
    }
    else if (NMU_TAR_FILE==file_flag)
    {
       //printf("	This is webs\n");
       //doSystem("rm /bin/%s",bn);
       system("killall -9 clish");
       system("killall -9 snmpd");
       system("killall -9 ccenter");
       system("rm /vispace/webs/webs");
    }
    else if (NMU_TAR_FILE==file_flag)
    {
       system("killall -9 clish");
       system("killall -9 snmpd");
       system("killall -9 ccenter");
    }
    else if (IMAGE_FILE==file_flag)
    {
       strcpy(savetopath,"/vispace/www/image");
    }
*/
    if (bn != NULL)
    {
      char buf[64];
      if (file_flag!=DATABASE_FILE)
        sprintf(buf,"%s/%s",savetopath,bn);
      else
        strcpy(buf,"/vispace/www/data/backup.db");
      fp = fopen(buf, "w+b");
    }
    //if ((fp = fopen((bn == NULL ? "/vispace/upldForm.bin" : bn), "w+b")) == NULL) {
    if (fp== NULL) {
    {
       websWrite(wp, T("File %s/%s open failed!<br>"),savetopath,bn);
       printf ("File %s open failed!<br>",savetopath);
    }
    } else {
        locWrite = 0;
        numLeft = wp->lenPostData;
        while (numLeft > 0) {
            numWrite = fwrite(&(wp->postData[locWrite]), sizeof(*(wp->postData)), numLeft, fp);
            if (numWrite < numLeft) {
                //websWrite(wp, T("File write failed.<br>  ferror=%d locWrite=%d numLeft=%d numWrite=%d Size=%d bytes<br>"), ferror(fp), locWrite, numLeft, numWrite, wp->lenPostData);
               websWrite(wp, T("File write failed.<br>  ferror=%d locWrite=%d numLeft=%d numWrite=%d Size=%d bytes<br>"), ferror(fp), locWrite, numLeft, numWrite, wp->lenPostData);
            break;
            }
            locWrite += numWrite;
            numLeft -= numWrite;
            //printf("left:%dbytes\n",numLeft);
            retCode=numLeft;
        }

        if (numLeft == 0) 
        {
            if (fclose(fp) != 0) 
            {
                websWrite(wp, T("File close failed.<br>  errno=%d locWrite=%d numLeft=%d numWrite=%d Size=%d bytes<br>"), errno, locWrite, numLeft, numWrite, wp->lenPostData);
            } 
            else 
            {
                #ifdef DEFAULT_LANG_CN
                 websWrite(wp, T("保存的文件大小 = %d 字节<br>文件 %s 上传成功!"), wp->lenPostData,bn);
                #else
                websWrite(wp, T("File Size Written = %d bytes<br>File %s upload successfully!"), wp->lenPostData,bn);
                #endif
                if (file_type==TAR_FILE_TYPE)
                {
                  doSystem("cd / && tar -zxvf %s",bn);
                  system("update");
                  sleep(3);
                  system("rm /*.tar.gz");
                  system("rm /bin/update");
                  //doSystem("rm %s/%s",savetopath,bn);
                  #ifdef DEFAULT_LANG_CN
                     websWrite(wp, T("<br/><a href='/upgrade2.htm'>返回</a>"));
                  #else
                     websWrite(wp, T("<br/><a href='/upgrade2.htm'>return</a>"));
                  #endif
                }
                else if (file_flag==UNIT_FILE)
                {
                   char n;
                   char_t *slot;
                   char slots[60]="",unit[8]="";
                   for (n=1;n<16;n++)
                   {
                     sprintf(unit,"u%d",n);
                     slot=websGetVar(wp, T(unit), T("0"));
                     if (!gstrcmp(slot,"on"))
                        sprintf(slots,"%s %d",slots,n);
                   }
                   //slots= websGetVar(wp, T("slots"), T("0"));
                   //printf("slots:%s\n",slots);
                   if (slots[0]!='\0')
                   {
                     pid_t status=doSystem("ftransfer -s\"%s\" -f %s",slots,bn);
                     if (WEXITSTATUS(status)!=0)
                     {
                       #ifdef DEFAULT_LANG_CN
                         websWrite(wp, T("<br/>单元盘升级失败"));
                       #else
                         websWrite(wp, T("<br/>Unit upgrade fail"));
                       #endif
                     }
                     else
                     {
                      #ifdef DEFAULT_LANG_CN
                       websWrite(wp, T("<br/>单元盘升级完毕"));
                      #else
                       websWrite(wp, T("<br/>Unit upgrade fail"));
                      #endif
                     }
                   }
                   #ifdef DEFAULT_LANG_CN
                       websWrite(wp, T("<br/><a href='/uupgrade2.htm'>返回</a>"));
                   #else
                       websWrite(wp, T("<br/><a href='/uupgrade2.htm'>return</a>"));
                   #endif
                    //printf("result=%d\n",system("ftransfer -f otu-3-5.bin"));
                   //printf("wifexited(status):%d\n", WIFEXITED(status));  
                   //printf("WEXITSTATUS(status):%d\n", WEXITSTATUS(status)); 
                }
                else if (file_flag==DATABASE_FILE)
                {
                  websWrite(wp, "<br/>Restore user data");
                  if (restoreUserData("backup.db"))
                      websWrite(wp, " fail!");
                  else
                       printf(" OK\n");
                  websWrite(wp, "<br/>Restore community");
  
                  if (restoreSnmpCoununity("backup.db"))
                      websWrite(wp, " fail!");
                  else
                       websWrite(wp," OK\n");
                 
                   websWrite(wp, "<br/>Restore alarm destination host");
                  if (restoreSnmpDestHost("backup.db"))
                      websWrite(wp, " fail!");
                  else
                      websWrite(wp," OK\n");

                 #ifdef DEFAULT_LANG_CN
                     websWrite(wp, T("<br><a href='/restoredb.asp'>返回</a>"));
                  #else
                     websWrite(wp, T("<br><a href='/restoredb.asp'>return</a>"));
                  #endif
                }
                else if (strstr(bn,".gif") ||strstr(bn,".jpg") ||strstr(bn,".png") )
                {
                   setVendorInfo(5,bn);
                }
            }
        } else {
            websWrite(wp, T("numLeft=%d locWrite=%d Size=%d bytes<br>"), numLeft, locWrite, wp->lenPostData);
        }
    }
    //printf("file_flag :%d\n",file_flag);
    if (WEBS_FILE==file_flag ||SNMPD_FILE==file_flag||CCENTER_FILE==file_flag||CLI_FILE==file_flag)
    {
       //printf("..chmod +x %s/%s",savetopath,bn);
       doSystem("chmod +x %s/%s",savetopath,bn);
    }
    if (SNMPD_FILE==file_flag ||SNMP_TAR_FILE==file_flag)
    {
       //printf("	This is webs2\n");
       //system("chmod +x /bin/snmpd");
       system("killall snmpd && snmpd -c /vispace/snmp/snmpd.conf &");
    }
    else if (CCENTER_FILE==file_flag)
    {
        system("killall ccenter && ccenter&");
    }
    else if (CLI_FILE==file_flag ||CLI_TAR_FILE==file_flag)
    {
       //printf("	This is webs2\n");
       //system("chmod +x /bin/snmpd");
       //system("clish");
    }
    #ifdef DEFAULT_LANG_CN
       if  (IMAGE_FILE!=file_flag)
       {
         //doSystem("chmod +x %s",savetopath);
         if  (WEBS_FILE==file_flag || WEBS_TAR_FILE==file_flag || NMU_TAR_FILE==file_flag)
         {
            
            websWrite(wp, T("<br/>系统正在重启...<br/>"));
            /*system("chmod +x /vispace/webs/webs");
            if (NMU_TAR_FILE==file_flag)
            {
              system("chmod +x /bin/snmpd");
              system("chmod +x /bin/clish");
              system("chmod +x /bin/ccenter");
            }
            */
         }
         else if  (SNMPD_FILE==file_flag ||SNMP_TAR_FILE==file_flag)
         {
           websWrite(wp, T("snmpd 正在重启..."));
           //system("chmod +x /bin/snmpd");
           //system("snmpd -c /vispace/snmp/snmpd.conf");
           websWrite(wp, T("snmpd 重启完毕"));
         }
         else if  (CLI_FILE==file_flag || CLI_TAR_FILE==file_flag)
         {
           websWrite(wp, T("CLI 正在重启..."));
           //system("chmod +x /bin/clish");
           //system("clish");
           websWrite(wp, T("clish 重启完毕"));
         }
         else if  (CCENTER_FILE==file_flag)
         {
           //system("chmod +x /bin/ccenter");
           websWrite(wp, T("ccenter 正在重启..."));
           //system("ccenter");
           websWrite(wp, T("ccenter 重启完毕"));
         }
       }
       //reboot(RB_AUTOBOOT);
    #else
       websWrite(wp, T("reboot..."));
    #endif
    websWrite(wp, T("</div>"));
    //printf("reboot...");
    websFooter(wp);
    //printf("reboot2...");
    websDone(wp, 200);
    if (WEBS_TAR_FILE==file_flag|| WEBS_FILE==file_flag || NMU_TAR_FILE==file_flag)
    {
      //printf("reboot2...");
      sleep(1);
      reboot(RB_AUTOBOOT);
    }

}

/******************************************************************************/
