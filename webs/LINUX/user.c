#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <um.h>
#include <emfdb.h>
#include <webs.h>
#define UM_TXT_FILENAME	T("/vispace/webs/umconfig.txt")
//arm-linux-gcc -o user user.c -I../ -L=./ -s libwebs.a
int saveUser()
{
if (umCommit(UM_TXT_FILENAME) != 0) {
               #ifdef DEFAULT_LANG_CN
                 printf("出错:不能保存用户配置.");
               #else
                 printf("ERROR: Unable to save user configuration.");
               #endif
	} else {
		//websWrite(wp, T("User configuration was saved successfully."));
               #ifdef DEFAULT_LANG_CN
                 printf("用户配置保存成功.");
               #else
                 printf("User configuration was saved successfully.");
               #endif
	}
}
int deleteUser(char *userid)
{
   //umOpen();
        //umRestore(T("umconfig.txt"));
   //umRestore(UM_TXT_FILENAME);
   //umRestore("/vispace/webs/umconfig.txt");
   if (umUserExists(userid) == FALSE) {
                #ifdef DEFAULT_LANG_CN
                   printf("出错: 用户 \"%s\" 不存在\n", userid);
               #else
                  printf("ERROR: User \"%s\" not found\n", userid);
               #endif
		
	} else if (umGetUserProtected(userid)) {
		printf("ERROR: User, \"%s\" is delete-protected.", userid);
	} else if (umDeleteUser(userid) != 0) {
                #ifdef DEFAULT_LANG_CN
                   printf("出错:不能删除用户, \"%s\" \n", userid);
               #else
                  printf("ERROR: Unable to delete user, \"%s\"\n", userid);
               #endif
		
	} else {
                #ifdef DEFAULT_LANG_CN
                   printf("用户, \"%s\" 删除成功.\n", userid);
               #else
                  printf("User, \"%s\" was successfully deleted.\n", userid);
               #endif
               saveUser();
	}
      //umClose();
}

void showUsers()
{
	char_t	*userid,*usergrp;
        char *user;
        short uselevel;
        bool_t enable;
	int	row;

	row = 0;
        //umOpen();
        //umRestore(T("umconfig.txt"));
        //umRestore(UM_TXT_FILENAME);
	userid = umGetFirstUser();
        if (NULL!=userid)
        {
          printf("umGetFirstUser found!\n");
          uselevel= umGetUserLevel(userid);
          usergrp=umGetUserGroup(userid);
          enable=umGetUserEnabled(userid);
        }
        else
            printf("Not user found!\n");
	while (userid) {
           printf("usergrp=%s ",usergrp);
           printf("userid =%s ",userid);
           printf("uselevel=%d\n",uselevel);
           
	   userid = umGetNextUser(userid);
           if (NULL!=userid)
           {
             usergrp=umGetUserGroup(userid);
             uselevel= umGetUserLevel(userid);
             enable=umGetUserEnabled(userid);
           }
	}
}
int main(int argc, char** argv)
{
  umOpen();
  umRestore(UM_TXT_FILENAME);
  showUsers();
  if (argc>2)
  {
    if (!strcmp(argv[1],"-d"))
    {
      deleteUser(argv[2]);
      showUsers();
    }
  }
  umClose();
}
