#ifndef _CONFDEFINE_H_
#define _CONFDEFINE_H_

#define CONF_DIR "./Conf/"
#define CONF_PATH(x) CONF_DIR#x

#define CONF_LOGIN CONF_PATH(Login.ini)
#define CONF_LOGIN_USERINFO "userinfo"
#define CONF_LOGIN_USERINFO_ID "userid"
#define CONF_LOGIN_USERINFO_PWD "password"
#define CONF_LOGIN_USERINFO_RMBPWD "rememberpwd"
#define CONF_LOGIN_USERINFO_AUTOLOGIN "autologin"
#define CONF_LOGIN_SERVERINFO "serverinfo"
#define CONF_LOGIN_SERVERINFO_ADDR "address"
#define CONF_LOGIN_SERVERINFO_PORT "port"

#endif
