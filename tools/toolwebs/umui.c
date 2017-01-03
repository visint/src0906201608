/*
 * umui.c -- User Management GoForm Processing
 *
 * Copyright (c) GoAhead Software Inc., 1995-2010. All Rights Reserved.
 *
 * See the file "license.txt" for usage and redistribution license requirements
 *
 */

/******************************** Description *********************************/

/*
 *	This module provides GoForm functions for User management
 */

/********************************* Includes ***********************************/

#include	"wsIntrn.h"
#include	"um.h"
#include	"../include/log.h"
#include <public.h>
/********************************* Defines ************************************/

#define		NONE_OPTION		T("<NONE>")
#define		MSG_START		T("<body><h2>")
#define		MSG_END			T("</h2></body>")

/**************************** Forward Declarations ****************************/

static void		formAddUser(webs_t wp, char_t *path, char_t *query);
static void		formDeleteUser(webs_t wp, char_t *path, char_t *query);
static void		formModifyPass(webs_t wp, char_t *path, char_t *query);
static void		formModifyUser(webs_t wp, char_t *path, char_t *query);
static void		formDisplayUser(webs_t wp, char_t *path, char_t *query);
static int		aspGenerateUserList(int eid, webs_t wp, 
									int argc, char_t **argv);

static void		formAddGroup(webs_t wp, char_t *path, char_t *query);
static void		formDeleteGroup(webs_t wp, char_t *path, char_t *query);
static int		aspGenerateGroupList(int eid, webs_t wp, 
									 int argc, char_t **argv);

static void		formAddAccessLimit(webs_t wp, char_t *path, char_t *query);
static void		formDeleteAccessLimit(webs_t wp, char_t *path, char_t *query);
static int		aspGenerateAccessLimitList(int eid, webs_t wp, 
										   int argc, char_t **argv);

static int		aspGenerateAccessMethodList(int eid, webs_t wp, 
											int argc, char_t **argv);
static int		aspGeneratePrivilegeList(int eid, webs_t wp, 
										 int argc, char_t **argv);
static int 		aspUserPrivilege(int eid, webs_t wp,int argc, char_t **argv);

static void		formSaveUserManagement(webs_t wp, char_t *path, char_t *query);
static void		formLoadUserManagement(webs_t wp, char_t *path, char_t *query);

static void		websMsgStart(webs_t wp);
static void		websMsgEnd(webs_t wp);

/*********************************** Code *************************************/
/*
 *	Set up the User Management form handlers
 */

void formDefineUserMgmt(void)
{
	websAspDefine(T("MakeGroupList"), aspGenerateGroupList);
	websAspDefine(T("MakeUserList"), aspGenerateUserList);
	websAspDefine(T("MakeAccessLimitList"), aspGenerateAccessLimitList);
	websAspDefine(T("MakeAccessMethodList"), aspGenerateAccessMethodList);
	websAspDefine(T("MakePrivilegeList"), aspGeneratePrivilegeList);
        websAspDefine(T("Privilege"),aspUserPrivilege);
	websFormDefine(T("AddUser"), formAddUser);
        websFormDefine(T("ModPass"),formModifyPass);
	websFormDefine(T("DeleteUser"), formDeleteUser);
        websFormDefine(T("ModifyUser"), formModifyUser);
	websFormDefine(T("DisplayUser"), formDisplayUser);
	websFormDefine(T("AddGroup"), formAddGroup);
	websFormDefine(T("DeleteGroup"), formDeleteGroup);
	websFormDefine(T("AddAccessLimit"), formAddAccessLimit);
	websFormDefine(T("DeleteAccessLimit"), formDeleteAccessLimit);

	websFormDefine(T("SaveUserManagement"), formSaveUserManagement);
	websFormDefine(T("LoadUserManagement"), formLoadUserManagement);
}

/******************************************************************************/
/*
 *  Add a user
 */

static void formAddUser(webs_t wp, char_t *path, char_t *query)
{
	char_t	*userid,*pass0, *pass1, *pass2, *group, *enabled, *ok,*isadd;
        //char_t	*userlevel;
	bool_t bDisable;
	int	nCheck;

	a_assert(wp);

	userid = websGetVar(wp, T("user"), T("")); 
        //userlevel = websGetVar(wp, T("level"), T(""));
	pass1 = websGetVar(wp, T("password"), T("")); 
	pass2 = websGetVar(wp, T("passconf"), T("")); 
	group = websGetVar(wp, T("group"), T("")); 
	enabled = websGetVar(wp, T("enabled"), T(""));
        //isadd= websGetVar(wp, T("isadd"), T(""));
	ok = websGetVar(wp, T("ok"), T("")); 

	//websHeader(wp);
      pageHeader(wp);
	websMsgStart(wp);
      #ifdef DEFAULT_LANG_CN
       pageMenu(wp,"添加用户");
      #else
        pageMenu(wp,"Add user");
      #endif
      //printf("Group=%s\n",group);
      websWrite(wp, T("<div align='center'>"));
	if (gstricmp(ok, T("ok")) != 0) {
         #if DEFAULT_LANG_CN
		websWrite(wp, T("添加用户被取消"));
         #else
                websWrite(wp, T("Add User Cancelled"));
         #endif
	} else if (gstrcmp(pass1, pass2) != 0) {
                #if DEFAULT_LANG_CN
		    websWrite(wp, T("确认密码不一致."));
                #else
                    websWrite(wp, T("Confirmation Password did not match."));
                #endif
	} else {
		if (enabled && *enabled && (gstrcmp(enabled, T("on")) == 0)) {
			bDisable = FALSE;
		} else {
			bDisable = TRUE;
		}

		nCheck = umAddUser(userid, pass1, group, 0, bDisable);
		if (nCheck != 0) {
			char_t * strError;

			switch (nCheck) {
			case UM_ERR_DUPLICATE:
                             #if DEFAULT_LANG_CN
                                strError = T("用户已存在.");
                             #else
                                strError = T("User already exists.");
                             #endif
				break;

			case UM_ERR_BAD_NAME:
                             #if DEFAULT_LANG_CN
                                 strError = T("用户名无效.");
                             #else
                                 strError = T("Invalid user name.");
                             #endif
				break;

			case UM_ERR_BAD_PASSWORD:
                             #if DEFAULT_LANG_CN
                                strError = T("密码无效.");  
                             #else
                                strError = T("Invalid password.");
                             #endif
				break;

			case UM_ERR_NOT_FOUND:
                             #if DEFAULT_LANG_CN
                                 strError = T("用户组无效或未选.");
                             #else
                                  strError = T("Invalid or unselected group.");
                             #endif
				break;

			default:
                             #if DEFAULT_LANG_CN
                               strError = T("Error writing user record.");
                             #else
                               strError = T("Error writing user record.");
                             #endif
			     break;
			}
                        #if DEFAULT_LANG_CN
                            websWrite(wp, T("不能添加用户: \"%s\".  %s"),
				userid, strError);
                        #else
                            websWrite(wp, T("Unable to add user, \"%s\".  %s"),
				userid, strError);  
                        #endif
		} else {
                        int level;
                        if (!gstrcmp(group,"admin"))
                            level=1;
                        else
                            level=2;
                        umSetUserLevel(userid, level);
                        #if DEFAULT_LANG_CN
                            websWrite(wp, T("用户: \"%s\" 添加成功."),
				userid); 
                        #else
                            websWrite(wp, T("User, \"%s\" was successfully added."),
				userid);
                        #endif
		}
	}
      #if DEFAULT_LANG_CN
      websWrite(wp, T("<h3><a class='white medium' href='/um/adduser.asp'>返回</a></h3></div>"));
      #else
        websWrite(wp, T("<h3><a class='white medium' href='/um/adduser.asp'>Return</a></h3></div>"));
      #endif
	websMsgEnd(wp);
	websFooter(wp);
	websDone(wp, 200);
}

static void formModifyPass(webs_t wp, char_t *path, char_t *query)
{
  char_t *userid, *pass0, *pass1, *pass2, *pass, *group, *enabled, *ok;
  userid = websGetVar(wp, T("user"), T("")); 
  pass0 = websGetVar(wp, T("oldpass"), T(""));
  pass1 = websGetVar(wp, T("password"), T("")); 
  pass2 = websGetVar(wp, T("passconf"), T("")); 
  //group = websGetVar(wp, T("group"), T("")); 
  enabled = websGetVar(wp, T("enabled"), T(""));
  ok = websGetVar(wp, T("ok"), T(""));
  //websHeader(wp);
  //websMsgStart(wp);
  pageHeader(wp);
  websMsgStart(wp);
  #if DEFAULT_LANG_CN
    pageMenu(wp,"修改密码");
  #else
    pageMenu(wp,"Modify Password");
  #endif
  websWrite(wp, T("<div align='center'>"));
  if (gstricmp(ok, T("ok")) != 0) {
      #if DEFAULT_LANG_CN
	websWrite(wp, T("修改密码被取消"));
      #else
        websWrite(wp, T("密码"));
      #endif
  } else if (gstrcmp(pass1, pass2) != 0){
       #if DEFAULT_LANG_CN
         websWrite(wp, T("密码不匹配."));
       #else
         websWrite(wp, T("Confirmation Password did not match."));
       #endif
  } else if (!umUserExists(userid)){
       #if DEFAULT_LANG_CN
         websWrite(wp, T("用户:%s不存在"),userid);
       #else
         websWrite(wp, T("User %s not exist"),userid);
       #endif
  }else if (!umUserExists(userid)){
       //websWrite(wp, T("User %s not exist"),userid);
       #if DEFAULT_LANG_CN
         websWrite(wp, T("用户:%s不存在"),userid);
       #else
         websWrite(wp, T("User %s not exist"),userid);
       #endif
  }else{
    //int n,passlen;
    char_t *upass;

    upass=umGetUserPassword(userid);
    //printf("%s:%s-%s\n",userid,pass0,upass);
    if (!gstrcmp(pass0,upass))
    {
        //websWrite(wp, T("Password is valid"));
        umSetUserPassword(userid,pass1);
        insertOperateLog(LOG_GROUP_SYS_PASSWORD_CONF,0,0,WebMode,mod,0,0);
        #if 1==DEFAULT_LANG_CN
         websWrite(wp, T("用户:%s密码修改成功"),userid);
        #else
          websWrite(wp, T("Password of \"%s\" was successfully modified.\n"),
				userid);
        #endif
   }
    else
       #ifdef DEFAULT_LANG_CN
         websWrite(wp, T("密码无效"),userid);
       #else
          websWrite(wp, T("Password is invalid"));
       #endif 
  }
 #if DEFAULT_LANG_CN
       websWrite(wp, T("<h3><a class='white medium' href='/um/pw.htm'>返回</a></h3></div>"));
 #else
       websWrite(wp, T("<h3><a class='white medium' href='/um/pw.htm'>Return</a></h3></div>"));
  #endif
  websMsgEnd(wp);
  websFooter(wp);
  websDone(wp, 200);
}

static void formModifyUser(webs_t wp, char_t *path, char_t *query)
{
  char_t *userid, *privilege,*enable;
  char group[32]="";
  //group = websGetVar(wp, T("group"), T(""));
  userid = websGetVar(wp, T("user"), T("")); 
  privilege = websGetVar(wp, T("privilege"), T("1"));
  enable= websGetVar(wp, T("enable"), T("0"));
  if (FALSE!=umUserExists(userid))
  {
    short level=atoi(privilege);
    if (level!=1)
    {
       level=0;
       strcpy(group,"admin");
    }
    else
    {
      level=2;
      strcpy(group,"guest");
    }
    //printf("userid:%s,group:%s\n",userid,group);
    umSetUserLevel(userid,level);
    umSetUserGroup(userid,group);
    if (!gstrcmp(enable,"1"))
      umSetUserEnabled(userid, TRUE);
    else 
      umSetUserEnabled(userid, FALSE);
  }
  pageHeader(wp);
  websMsgStart(wp);
  #ifdef DEFAULT_LANG_CN
  pageMenu(wp,"修改用户");
  #else
  pageMenu(wp,"Modify user");
  #endif
  websWrite(wp, T("<div align='center'>"));
  #ifdef DEFAULT_LANG_CN
    websWrite(wp, T("<h3><a class='white medium' href='/um/user.asp'>返回</a></h3></div>"));
  #else
   websWrite(wp, T("<h3><a class='white medium' href='/um/user.asp'>Return</a></h3></div>"));
  #endif
  websMsgEnd(wp);
  websFooter(wp);
  websDone(wp, 200);
}
/******************************************************************************/
/*
 *  Delete a user
 */

static void formDeleteUser(webs_t wp, char_t *path, char_t *query)
{
	char_t	*userid, *ok;

	a_assert(wp);

	userid = websGetVar(wp, T("user"), T("")); 
	ok = websGetVar(wp, T("ok"), T("")); 

	//websHeader(wp);
      pageHeader(wp);
	websMsgStart(wp);
      
      #ifdef DEFAULT_LANG_CN
          pageMenu(wp,"删除用户");
        #else
          pageMenu(wp,"Delete user");
        #endif
      websWrite(wp, T("<div align='center'>"));
	if (gstricmp(ok, T("ok")) != 0) {
               #ifdef DEFAULT_LANG_CN
                    websWrite(wp, T("删除用户被取消."));
               #else
                    websWrite(wp, T("Delete User Cancelled"));
               #endif
		
	} else if (umUserExists(userid) == FALSE) {
                #ifdef DEFAULT_LANG_CN
                   websWrite(wp, T("出错: 用户 \"%s\" 不存在"), userid);
               #else
                  websWrite(wp, T("ERROR: User \"%s\" not found"), userid);
               #endif
		
	} else if (umGetUserProtected(userid)) {
                #ifdef DEFAULT_LANG_CN
                   websWrite(wp, T("出错: 用户, \"%s\" 受保护."), userid);
               #else
                  websWrite(wp, T("ERROR: User, \"%s\" is delete-protected."), userid);
               #endif
		
	} else if (umDeleteUser(userid) != 0) {
               #ifdef DEFAULT_LANG_CN
                   websWrite(wp, T("出错:不能删除用户, \"%s\" "), userid);
               #else
                  websWrite(wp, T("ERROR: Unable to delete user, \"%s\" "), userid);
               #endif
		
	} else {
                #ifdef DEFAULT_LANG_CN
                   websWrite(wp, T("用户, \"%s\" 删除成功."), userid);
               #else
                  websWrite(wp, T("User, \"%s\" was successfully deleted."), userid);
               #endif
		
                insertOperateLog(LOG_GROUP_SYS_USER_CONF,0,0,WebMode,del,0,0);
	}
      #ifdef DEFAULT_LANG_CN
      websWrite(wp, T("<h3><a class='white medium' href='/um/user.asp'>返回</a></h3></div>"));
      #else
      websWrite(wp, T("<h3><a class='white medium' href='/um/user.asp'>Return</a></h3></div>"));
      #endif
	websMsgEnd(wp);
	websFooter(wp);
	websDone(wp, 200);
}
short checkPrivilege(webs_t wp,char *page)
{
  short  privilege=umGetUserPrivilege(wp->userName);
  if (privilege==1)
  {
    pageHeader(wp);
    websMsgStart(wp);
    #ifdef DEFAULT_LANG_CN
      websWrite(wp, T("<h3>操作权限不够</h3>"));
      if (page!=NULL)
        websWrite(wp, T("<h3><a class='white medium' href='%s'>返回</a></h3>"),page);
    #else
      websWrite(wp, T("<h3><a class='white medium' href='/um/user.asp'>Return</a></h3>"));
     #endif
    websMsgEnd(wp);
    websFooter(wp);
    websDone(wp, 200);
  }
  return privilege;
}
/******************************************************************************/
/*
 *  Display the user info
 */

static void formDisplayUser(webs_t wp, char_t *path, char_t *query)
{
	char_t	*userid, *ok, *temp;
	bool_t	enabled;

	a_assert(wp);

	userid = websGetVar(wp, T("user"), T("")); 
	ok = websGetVar(wp, T("ok"), T("")); 

	websHeader(wp);
	websWrite(wp, T("<body>"));

	if (gstricmp(ok, T("ok")) != 0) {
                #ifdef DEFAULT_LANG_CN
                   websWrite(wp, T("显示用户被取消"));
               #else
                  websWrite(wp, T("Display User Cancelled"));
               #endif
		
	} else if (umUserExists(userid) == FALSE) {
		websWrite(wp, T("ERROR: User <b>%s</b> not found.\n"), userid);
	} else {
		websWrite(wp, T("<h2>User ID: <b>%s</b></h2>\n"), userid);
		temp = umGetUserGroup(userid);
		websWrite(wp, T("<h3>User Group: <b>%s</b></h3>\n"), temp);
		enabled = umGetUserEnabled(userid);
		websWrite(wp, T("<h3>Enabled: <b>%d</b></h3>\n"), enabled);
	}

	websWrite(wp, T("</body>\n"));
	websFooter(wp);
	websDone(wp, 200);
}


/******************************************************************************/
/*
 *  Generate HTML to create a list box containing the users
 */

static int aspGenerateUserList(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t	*userid,*usergrp;
        short uselevel,privilege;
        bool_t enable,protected;
	int	row, nBytesSent;

	a_assert(wp);

	//nBytes = websWrite(wp, 
	//	T("<SELECT NAME=\"user\" SIZE=\"6\" TITLE=\"Select a User\">"));
	row = 0;
	userid = umGetFirstUser();
        /*if (NULL!=userid)
        {
          uselevel= umGetUserLevel(userid);
          usergrp=umGetUserGroup(userid);
          enable=umGetUserEnabled(userid);
          protected=umGetUserProtected(userid);
        }
        */
	nBytesSent = 0;

	while (userid) {
           //uselevel= umGetUserLevel(userid);
           privilege=umGetUserPrivilege(userid);
           usergrp=umGetUserGroup(userid);
           enable=umGetUserEnabled(userid);
           protected=umGetUserProtected(userid);

           nBytesSent += websWrite(wp, T("<TR><TD>%s</TD>"),userid);
           nBytesSent += websWrite(wp, T("<TD>%s</TD>"),usergrp);
           //nBytesSent += websWrite(wp, T("<TD>"));
           
           if (TRUE!=protected)
           {
             //nBytesSent += websWrite(wp, T(" disabled=true "));
           nBytesSent += websWrite(wp, T("<TD><select id='ul_%s'"),userid);
           nBytesSent += websWrite(wp, T("><option value='4'"));
           if (4==privilege)
               nBytesSent += websWrite(wp, T("selected='selected'"));
           //printf("user:%d protected:%d\n",userid,protected);
           
           nBytesSent += websWrite(wp, ">");
           #ifdef DEFAULT_LANG_CN
             nBytesSent += websWrite(wp, "管理用户");
           #else
             nBytesSent += websWrite(wp, "Admin user");
           #endif
           nBytesSent += websWrite(wp, "<option value='1'");
           if (1==privilege)
               nBytesSent += websWrite(wp, "selected='selected'");
 
           nBytesSent += websWrite(wp, T(">"));
           #ifdef DEFAULT_LANG_CN
             nBytesSent += websWrite(wp, "普通用户");
           #else
             nBytesSent += websWrite(wp, "Normal user");
           #endif
           //printf("userid=%s level=%d\n",userid,privilege);
           nBytesSent += websWrite(wp, T("</select></TD><TD><select id='en_%s'> <option value='1'"),userid);
           if (TRUE==enable)
               nBytesSent += websWrite(wp, T("selected='selected'"));
           
           nBytesSent += websWrite(wp, ">");
           #ifdef DEFAULT_LANG_CN
              nBytesSent += websWrite(wp, "启用");
           #else
                nBytesSent += websWrite(wp, "Enable");
           #endif
           nBytesSent += websWrite(wp, "<option value='2'");
           if (FALSE==enable)
               nBytesSent += websWrite(wp, T("selected='selected'"));
 
           nBytesSent += websWrite(wp, ">");
           #ifdef DEFAULT_LANG_CN
                nBytesSent += websWrite(wp, "禁用");
           #else
                nBytesSent += websWrite(wp, "Disable");
           #endif
            if (umGetUserPrivilege(wp->userName)!=1)
            {
              if (gstrcmp(userid,"admin"))
              nBytesSent += websWrite(wp, T("</select></TD><TD><a href='#'><img src='../../images/pen.gif' onclick=\"mod_user(1,'%s');\"/></a><a href='#'><img src='../../images/del.gif' onclick=\"mod_user(2,'%s');\"/></a></TD></TR>"),userid,userid);
              else
              nBytesSent += websWrite(wp, T("</select></TD><TD><a href='#'><img src='../../images/pen.gif' onclick=\"mod_user(1,'%s');\"/></a></TD></TR>"),userid,userid);
            }
           }
           else
           {
              nBytesSent += websWrite(wp, T("<TD>"));
              if (1==uselevel)
              {
                
                #ifdef DEFAULT_LANG_CN
                nBytesSent += websWrite(wp, "管理用户");
                 #else
                nBytesSent += websWrite(wp, "Admin user");
                #endif
              }
              else
              {
                
                #ifdef DEFAULT_LANG_CN
                 nBytesSent += websWrite(wp, T("普通用户"));
                #else
                 nBytesSent += websWrite(wp, "Normal user");
                #endif
              }
                
              nBytesSent += websWrite(wp, T("</TD><TD>"));
              if (FALSE!=enable)
              {
                #ifdef DEFAULT_LANG_CN
                nBytesSent += websWrite(wp, "启用");
                 #else
                nBytesSent += websWrite(wp, "Enable");
                #endif
               }
              else
              {
                #ifdef DEFAULT_LANG_CN
                nBytesSent += websWrite(wp, "禁用");
                 #else
                nBytesSent += websWrite(wp, "Disable");
                #endif
               }
              nBytesSent += websWrite(wp, T("</TD><TD></TD>"));
           }
           nBytesSent += websWrite(wp, T("</TR>"));
	   userid = umGetNextUser(userid);
           /*if (NULL!=userid)
           {
             usergrp=umGetUserGroup(userid);
             uselevel= umGetUserLevel(userid);
             enable=umGetUserEnabled(userid);
             protected=umGetUserProtected(userid);
           }
           */
	   //nBytesSent += nBytes;
	}

	//nBytesSent += websWrite(wp, T("</SELECT>"));

	return nBytesSent;
}

/******************************************************************************/
/*
 *  Add a group
 */

static void formAddGroup(webs_t wp, char_t *path, char_t *query)
{
	char_t			*group, *enabled, *privilege, *method, *ok, *pChar;
	int				nCheck;
	short			priv;
	accessMeth_t	am;
	bool_t			bDisable;

	a_assert(wp);

	group = websGetVar(wp, T("group"), T("")); 
	method = websGetVar(wp, T("method"), T("")); 
	enabled = websGetVar(wp, T("enabled"), T("")); 
	privilege = websGetVar(wp, T("privilege"), T("")); 
	ok = websGetVar(wp, T("ok"), T("")); 

	//websHeader(wp);
      pageHeader(wp);
	websMsgStart(wp);
      
      #ifdef DEFAULT_LANG_CN
       pageMenu(wp,"添加用户组");
       #else
       pageMenu(wp,"Add user group");
      #endif
      websWrite(wp, T("<div align='center'>"));
	if (gstricmp(ok, T("ok")) != 0) {
                #ifdef DEFAULT_LANG_CN
                   websWrite(wp, T("添加用户组被取消."));
               #else
                  websWrite(wp, T("Add Group Cancelled."));
               #endif
		
	} else if ((group == NULL) || (*group == 0)) {
               #ifdef DEFAULT_LANG_CN
                   websWrite(wp, T("用户组名不能为空."));
               #else
                  websWrite(wp, T("No Group Name was entered."));
               #endif
		
	} else if (umGroupExists(group)) {
                #ifdef DEFAULT_LANG_CN
                   websWrite(wp, T("出错: 用户组, \"%s\" 已存在."), group);
               #else
                  websWrite(wp, T("ERROR: Group, \"%s\" already exists."), group);
               #endif
		
	} else {
		if (privilege && *privilege) {
/*
 *			privilege is a mulitple <SELECT> var, and must be parsed.
 *			Values for these variables are space delimited.
 */
			priv = 0;
			for (pChar = privilege; *pChar; pChar++) {
				if (*pChar == ' ') {
					*pChar = '\0';
					priv |= gatoi(privilege);
					*pChar = ' ';
					privilege = pChar + 1;
				}
			}
			priv |= gatoi(privilege);
		} else {
			priv = 0;
		}

		if (method && *method) {
			am = (accessMeth_t) gatoi(method);
		} else {
			am = AM_FULL;
		}

		if (enabled && *enabled && (gstrcmp(enabled, T("on")) == 0)) {
			bDisable = FALSE;
		} else {
			bDisable = TRUE;
		}
                websWrite(wp, T("<div align='center'>"));
		nCheck = umAddGroup(group, priv, am, 0, bDisable);
		if (nCheck != 0) {
                   #ifdef DEFAULT_LANG_CN
                     websWrite(wp, T("不能添加用户组, \"%s\", code: %d "),
				group, nCheck);
                   #else
                    websWrite(wp, T("Unable to add group, \"%s\", code: %d "),
				group, nCheck);
                   #endif
			
		} else {
                        #ifdef DEFAULT_LANG_CN
                           websWrite(wp, T("用户组, \"%s\" 添加成功."),group);
                        #else
                           websWrite(wp, T("Group, \"%s\" was successfully added."),group);
                        #endif
			
                        insertOperateLog(LOG_GROUP_SYS_USER_GROUP_CONF,0,0,WebMode,add,0,0);
		}
	}
      #ifdef DEFAULT_LANG_CN
      websWrite(wp, T("<h3><a class='white medium' href='/um/ugroup.asp'>返回</a></h3></div>"));
      #else
        websWrite(wp, T("<h3><br/><a class='white medium' href='/um/ugroup.asp'>Return</a></h3></div>"));
      #endif
	websMsgEnd(wp);
	websFooter(wp);
	websDone(wp, 200);
}

/******************************************************************************/
/*
 *  Delete a group
 */

static void formDeleteGroup(webs_t wp, char_t *path, char_t *query)
{
	char_t	*group, *ok;

	a_assert(wp);

	group = websGetVar(wp, T("group"), T("")); 
	ok = websGetVar(wp, T("ok"), T("")); 

	//websHeader(wp);
        pageHeader(wp);
	  websMsgStart(wp);
        #ifdef DEFAULT_LANG_CN
          pageMenu(wp,"删除用户组");
        #else
          pageMenu(wp,"Delete user group");
        #endif
        websWrite(wp, T("<div align='center'>"));
	if (gstricmp(ok, T("ok")) != 0) {
               #ifdef DEFAULT_LANG_CN
                   websWrite(wp, T("删除用户组被取消."));
               #else
                  websWrite(wp, T("Delete Group Cancelled."));
               #endif
	
	} else if ((group == NULL) || (*group == '\0')) {
               #ifdef DEFAULT_LANG_CN
                    websWrite(wp, T("出错: 没有选择用户组."));
               #else
                    websWrite(wp, T("ERROR: No group was selected."));
               #endif
		
	} else if (umGetGroupProtected(group)) {
                #ifdef DEFAULT_LANG_CN
                 websWrite(wp, T("出错: 用户组, \"%s\" 删除受保护."), group);
               #else
                  websWrite(wp, T("ERROR: Group, \"%s\" is delete-protected."), group);
               #endif
		
	} else if (umGetGroupInUse(group)) {
               #ifdef DEFAULT_LANG_CN
                  websWrite(wp, T("出错: 用户组:\"%s\"正在使用中."),group);
               #else
                  websWrite(wp, T("ERROR: Group, \"%s\" is being used."),group);
               #endif
		
	} else if (umDeleteGroup(group) != 0) {
                #ifdef DEFAULT_LANG_CN
                  websWrite(wp, T("出错: 不能删除用户组, \"%s\" "), group);
               #else
                  websWrite(wp, T("ERROR: Unable to delete group, \"%s\" "), group);
               #endif
		
	} else {
                #ifdef DEFAULT_LANG_CN
                 websWrite(wp, T("用户组, \"%s\" 删除成功."), group);
                #else
                 websWrite(wp, T("Group, \"%s\" was successfully deleted."), group);
                #endif
		
                insertOperateLog(LOG_GROUP_SYS_USER_GROUP_CONF,0,0,WebMode,del,0,0);
	}
        #ifdef DEFAULT_LANG_CN
           websWrite(wp, T("<h3><a class='white medium' href='/um/ugroup.asp'>返回</a></h3></div>"));
        #else
          websWrite(wp, T("<h3><a class='white medium' href='/um/ugroup.asp'>Return</a></h3></div>"));
        #endif
	websMsgEnd(wp);
	websFooter(wp);
	websDone(wp, 200);
}

/******************************************************************************/
/*
 *  Generate HTML to create a list box containing the groups
 */

static int aspGenerateGroupList(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t	*group,*seltype;
	int	row, nBytesSent, nBytes;

	a_assert(wp);

	row = 0;
	nBytesSent = 0;
        
        if (ejArgs(argc, argv, T("%s %s"), &seltype) < 1) {
	websError(wp, 400, T("Insufficient args\n"));
	return -1;
       }
       if (gstrcmp(seltype,"2"))
      {
        nBytes = websWrite(wp, 
		T("<SELECT NAME=\"group\"  id=\"group\" SIZE=\"1\" TITLE=\"Select a Group\">"));

    //Add a special "<NONE>" element to allow de-selection
 
	nBytes = websWrite(wp, T("<OPTION VALUE=\"\">[NONE]\n"));

	group = umGetFirstGroup();
	while (group && (nBytes > 0)) {
		nBytes = websWrite(wp, T("<OPTION VALUE=\"%s\">%s\n"), group, group);
		group = umGetNextGroup(group);
		nBytesSent += nBytes;
	}
         
	nBytesSent += websWrite(wp, T("</SELECT>"));
      }
     else
     {
        nBytes = websWrite(wp, T("\n"));
        group = umGetFirstGroup();
	while (group && (nBytes > 0)) {
		nBytes = websWrite(wp, T("<TR><TD>%s</TD><TD colspan='2'><a href ='/goform/DeleteGroup?group=%s&ok=OK'>删除</a></TD></TR>\n"), group,group);
		group = umGetNextGroup(group);
		nBytesSent += nBytes;
	}
    }
	return nBytesSent;
}

/******************************************************************************/
/*
 *  Add an access limit
 */

static void formAddAccessLimit(webs_t wp, char_t *path, char_t *query)
{
	char_t			*url, *method, *group, *secure, *ok;
	int		        nCheck;
	accessMeth_t	am;
	short			nSecure;

	a_assert(wp);

	url = websGetVar(wp, T("url"), T("")); 
	group = websGetVar(wp, T("group"), T("")); 
	method = websGetVar(wp, T("method"), T("")); 
	secure = websGetVar(wp, T("secure"), T("")); 
	ok = websGetVar(wp, T("ok"), T("")); 

	//websHeader(wp);
	//websMsgStart(wp);
        pageHeader(wp);
	websMsgStart(wp);
      #ifdef DEFAULT_LANG_CN
      pageMenu(wp,"添加访问权限");
      #else
        pageMenu(wp,"Add access limit");
      #endif
      websWrite(wp, T("<div align='center'>"));
	if (gstricmp(ok, T("ok")) != 0) {
                #ifdef DEFAULT_LANG_CN
                 websWrite(wp, T("添加访问权限被取消."),url);
               #else
                 websWrite(wp, T("Add Access Limit Cancelled."));
               #endif
		
	} else if ((url == NULL) || (*url == 0)) {
               #ifdef DEFAULT_LANG_CN
                 websWrite(wp, T("出错:URL 不能为空."));
               #else
                 websWrite(wp, T("ERROR:No URL was entered."));
               #endif
		
	} else if (umAccessLimitExists(url)) {
                #ifdef DEFAULT_LANG_CN
                 websWrite(wp, T("出错:[%s] 访问权限已存在."),
			url);
               #else
                 websWrite(wp, T("ERROR:An Access Limit for [%s] already exists."),
			url);
               #endif
		
	} else {
		if (method && *method) {
			am = (accessMeth_t) gatoi(method);
		} else {
			am = AM_FULL;
		}

		if (secure && *secure) {
			nSecure = (short) gatoi(secure);
		} else {
			nSecure = 0;
		}

		nCheck = umAddAccessLimit(url, am, nSecure, group);
		if (nCheck != 0) {
                #ifdef DEFAULT_LANG_CN
                     websWrite(wp, T("访问权限[%s]不能删除"),	url);
                #else
                     websWrite(wp, T("Unable to add Access Limit for [%s]"),	url);
                #endif
			
		} else {
                   #ifdef DEFAULT_LANG_CN
                       websWrite(wp, T("[%s]访问权限添加成功."),url);
                   #else
                       websWrite(wp, T("Access limit for [%s], was successfully added."),url);
                   #endif
                        insertOperateLog(LOG_GROUP_SYS_USER_ACL_CONF,0,0,WebMode,add,0,0);
		}
	}
      #ifdef DEFAULT_LANG_CN
        websWrite(wp, T("<h3><a class='white medium' href='/um/limit.asp'>返回</a></h3></div>"));
      #else
        websWrite(wp, T("<h3><a class='white medium' href='/um/limit.asp'>Return</a></h3></div>"));
      #endif
	websMsgEnd(wp);
	websFooter(wp);
	websDone(wp, 200);
}

/******************************************************************************/
/*
 *  Delete an Access Limit
 */

static void formDeleteAccessLimit(webs_t wp, char_t *path, char_t *query)
{
	char_t	*url, *ok;

	a_assert(wp);

	url = websGetVar(wp, T("url"), T("")); 
	ok = websGetVar(wp, T("ok"), T("")); 

	pageHeader(wp);
	websMsgStart(wp);
      pageMenu(wp,"删除访问权限");
      websWrite(wp, T("<div align='center'>"));

	if (gstricmp(ok, T("ok")) != 0) {
               #ifdef DEFAULT_LANG_CN
                 websWrite(wp, T("访问权限删除被取消"));
               #else
                 websWrite(wp, T("Delete Access Limit Cancelled"));
               #endif
		
	} else if (umDeleteAccessLimit(url) != 0) {
                #ifdef DEFAULT_LANG_CN
                 websWrite(wp, T("出错: 访问权限 [%s]不能删除"), 
			url);
               #else
                 websWrite(wp, T("ERROR: Unable to delete Access Limit for [%s]"), 
			url);
               #endif
		
	} else {
                #ifdef DEFAULT_LANG_CN
                 websWrite(wp, T(" [%s] 访问权限删除成功."),url);
               #else
                 websWrite(wp, T("Access Limit for [%s], was successfully deleted."),url);
               #endif
		    
                insertOperateLog(LOG_GROUP_SYS_USER_ACL_CONF,0,0,WebMode,del,0,0);
	}
        #ifdef DEFAULT_LANG_CN
        websWrite(wp, T("<h3><a class='white medium' href='/um/limit.asp'>返回</a></h3></div>"));
        #else
        websWrite(wp, T("<h3><a class='white medium' href='/um/limit.asp'>Return</a></h3></div>"));
        #endif
	websMsgEnd(wp);
	websFooter(wp);
	websDone(wp, 200);
}

/******************************************************************************/
/*
 *  Generate HTML to create a list box containing the access limits
 */

static int aspGenerateAccessLimitList(int eid, webs_t wp, 
									  int argc, char_t **argv)
{
	char_t	*url;
        char page[30]="";
	int		row, nBytesSent, nBytes;

	a_assert(wp);

	row = nBytesSent = 0;
	url = umGetFirstAccessLimit();

        nBytes = websWrite(wp, T("\n"));
        while (url && (nBytes > 0)) {
        #ifdef DEFAULT_LANG_CN
                if (!gstrcmp(url,"/"))
                   strcpy(page,"全部");
                else if (!gstrcmp(url,"/index.htm"))
                   strcpy(page,"主页");
                else if (!gstrcmp(url,"/sys_info.asp"))
                   strcpy(page,"系统信息");
                else if (!gstrcmp(url,"/alarm_list.asp"))
                   strcpy(page,"告警管理");
                else if (!gstrcmp(url,"/module_list.asp"))
                   strcpy(page,"模块列表");
                else if (!gstrcmp(url,"/snmpconf.asp"))
                   strcpy(page,"SNMP配置");
                else if (!gstrcmp(url,"/ip.asp"))
                   strcpy(page,"系统管理");
                else if (!gstrcmp(url,"/optlog.asp"))
                   strcpy(page,"日志管理");
                else if (!gstrcmp(url,"/um/"))
                   strcpy(page,"用户管理");
                else
                   strcpy(page,url);
		nBytes = websWrite(wp, T("<TR><TD>%s</TD><TD>%s</TD><TD><a href ='/goform/DeleteAccessLimit?url=%s&ok=OK'>删除</a></TD></TR>\n"), umGetAccessLimitGroup(url), page,url);
        #else
             if (!gstrcmp(url,"/"))
                   strcpy(page,"All");
                else if (!gstrcmp(url,"/index.htm"))
                   strcpy(page,"Home");
                else if (!gstrcmp(url,"/sys_info.asp"))
                   strcpy(page,"System info");
                else if (!gstrcmp(url,"/alarm_list.asp"))
                   strcpy(page,"Alarm management");
                else if (!gstrcmp(url,"/module_list.asp"))
                   strcpy(page,"Module list");
                else if (!gstrcmp(url,"/snmpconf.asp"))
                   strcpy(page,"SNMP COnfigure");
                else if (!gstrcmp(url,"/ip.asp"))
                   strcpy(page,"System management");
                else if (!gstrcmp(url,"/optlog.asp"))
                   strcpy(page,"Log management");
                else if (!gstrcmp(url,"/um/"))
                   strcpy(page,"User management");
                else
                   strcpy(page,url);
		nBytes = websWrite(wp, T("<TR><TD>%s</TD><TD>%s</TD><TD><a href ='/goform/DeleteAccessLimit?url=%s&ok=OK'>Delete</a></TD></TR>\n"), umGetAccessLimitGroup(url), page,url);
        #endif
		url = umGetNextAccessLimit(url);
		nBytesSent += nBytes;
	}
	//

	return nBytesSent;
}

/******************************************************************************/
/*
 *  Generate HTML to create a list box containing the access methods
 */

static int aspGenerateAccessMethodList(int eid, webs_t wp, 
									   int argc, char_t **argv)
{
	int		nBytes;

	a_assert(wp);

	nBytes = websWrite(wp, 
		T("<SELECT NAME=\"method\" SIZE=\"3\" TITLE=\"Select a Method\">"));
	nBytes += websWrite(wp, T("<OPTION VALUE=\"%d\">FULL ACCESS\n"), 
		AM_FULL);
	nBytes += websWrite(wp, T("<OPTION VALUE=\"%d\">BASIC ACCESS\n"), 
		AM_BASIC);
	nBytes += websWrite(wp, T("<OPTION VALUE=\"%d\" SELECTED>DIGEST ACCESS\n"), 
		AM_DIGEST);
	nBytes += websWrite(wp, T("<OPTION VALUE=\"%d\">NO ACCESS\n"), 
		AM_NONE);
	nBytes += websWrite(wp, T("</SELECT>"));

	return nBytes;
}

/******************************************************************************/
/*
 *  User Privilege
 */

static int aspUserPrivilege(int eid, webs_t wp, int argc, char_t **argv)
{
  /*
  short privilege;
  char_t *ug=umGetUserGroup(wp->userName);
  if (ug!=NULL)
  {
     privilege=umGetGroupPrivilege(ug);
  }
  else
      privilege=0;
 */
  return websWrite(wp, T("var privilege=%d;\n"), umGetUserPrivilege(wp->userName));
}
/******************************************************************************/
/*
 *  Generate HTML to create a list box containing privileges
 */

static int aspGeneratePrivilegeList(int eid, webs_t wp, 
									int argc, char_t **argv)
{
	int		nBytes;

	a_assert(wp);

	nBytes = websWrite(wp, T("<SELECT NAME=\"privilege\" SIZE=\"3\" "));
	nBytes += websWrite(wp, T("MULTIPLE TITLE=\"Choose Privileges\">"));
	nBytes += websWrite(wp, T("<OPTION VALUE=\"%d\">READ\n"), PRIV_READ);
	nBytes += websWrite(wp, T("<OPTION VALUE=\"%d\">EXECUTE\n"), PRIV_WRITE);
	nBytes += websWrite(wp, T("<OPTION VALUE=\"%d\">ADMINISTRATE\n"), 
		PRIV_ADMIN);
	nBytes += websWrite(wp, T("</SELECT>"));

	return nBytes;
}

/******************************************************************************/
/*
 *  Save the user management configuration to a file
 */

static void formSaveUserManagement(webs_t wp, char_t *path, char_t *query)
{
	char_t	*ok;

	a_assert(wp);

	ok = websGetVar(wp, T("ok"), T("")); 

	//websHeader(wp);
        pageHeader(wp);
	  websMsgStart(wp);
        pageMenu(wp,"保存用户信息");
        websWrite(wp, T("<div align='center'>"));
	if (gstricmp(ok, T("ok")) != 0) {
                #ifdef DEFAULT_LANG_CN
                 websWrite(wp, T("保存被取消."));
               #else
                 websWrite(wp, T("Save Cancelled."));
               #endif
		
	} else if (umCommit(NULL) != 0) {
               #ifdef DEFAULT_LANG_CN
                 websWrite(wp, T("出错:不能保存用户配置."));
               #else
                 websWrite(wp, T("ERROR: Unable to save user configuration."));
               #endif
	} else {
		//websWrite(wp, T("User configuration was saved successfully."));
               #ifdef DEFAULT_LANG_CN
                 websWrite(wp, T("用户配置保存成功."));
               #else
                 websWrite(wp, T("User configuration was saved successfully."));
               #endif
	}
      #ifdef DEFAULT_LANG_CN
      websWrite(wp, T("<h3><a class='white medium' href='/um/savecfg.asp'>返回</a></h3></div>"));
      #else
          websWrite(wp, T("<h3><a class='white medium' href='/um/savecfg.asp'>Return</a></h3></div>"));
      #endif
	websMsgEnd(wp);
	websFooter(wp);
	websDone(wp, 200);
}

/******************************************************************************/
/*
 *  Load the user management configuration from a file
 */

static void formLoadUserManagement(webs_t wp, char_t *path, char_t *query)
{
	char_t	*ok;

	a_assert(wp);

	ok = websGetVar(wp, T("ok"), T("")); 

	websHeader(wp);
	websMsgStart(wp);

	if (gstricmp(ok, T("ok")) != 0) {
		websWrite(wp, T("Load Cancelled."));
	} else if (umRestore(NULL) != 0) {
                #ifdef DEFAULT_LANG_CN
                 websWrite(wp, T("出错:不能保存用户配置."));
               #else
                 websWrite(wp, T("ERROR: Unable to save user configuration."));
               #endif
		websWrite(wp, T("ERROR: Unable to load user configuration."));
	} else {
                #ifdef DEFAULT_LANG_CN
                 websWrite(wp, T("用户配置重新加载成功."));
               #else
                 websWrite(wp, T("User configuration was re-loaded successfully."));
               #endif
	}

	websMsgEnd(wp);
	websFooter(wp);
	websDone(wp, 200);
}

/******************************************************************************/
/*
 *  Message start and end convenience functions
 */

static void	websMsgStart(webs_t wp)
{
	websWrite(wp, MSG_START);
}

static void	websMsgEnd(webs_t wp)
{
	websWrite(wp, MSG_END);
}

/******************************************************************************/

