
/* query data from table "tbATUHandOver" */

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlext.h>
#include <sqltypes.h>

SQLHENV V_OD_Env;	 // Handle ODBC environment
SQLHSTMT V_OD_hstmt; // Handle statement
SQLHDBC V_OD_hdbc;	 // Handle connection
SQLCHAR SSECTOR_ID[50], NSECTOR_ID[50];
SQLINTEGER HOATT;
SQLINTEGER V_OD_erg, V_OD_buffer, V_OD_err, V_OD_id;

int main(int argc, char* argv[])
{
	// 申请环境句柄
	V_OD_erg = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &V_OD_Env);
	if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
	{
		printf("Error AllocHandle\n");
		exit(0);
	}

	// 设置环境属性（版本信息）
	SQLSetEnvAttr(V_OD_Env, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

	// 申请连接句柄
	V_OD_erg = SQLAllocHandle(SQL_HANDLE_DBC, V_OD_Env, &V_OD_hdbc);
	if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
	{
		SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
		exit(0);
	}

	// 设置连接属性
	// SQLSetConnectAttr(V_OD_hdbc, SQL_ATTR_AUTOCOMMIT, SQL_AUTOCOMMIT_ON, 0);

	// 连接数据源
	V_OD_erg = SQLConnect(V_OD_hdbc, (SQLCHAR*)"huaweicloud", SQL_NTS,
		(SQLCHAR*)"bupt2019dbs37", SQL_NTS, (SQLCHAR*)"bupt2019dbs37@123", SQL_NTS);
	if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
	{
		printf("Error SQLConnect %d\n", V_OD_erg);
		SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
		exit(0);
	}
	printf("Connected !\n");

	// 设置语句属性
	SQLSetStmtAttr(V_OD_hstmt, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER*)3, 0);

	// 申请语句句柄
	SQLAllocHandle(SQL_HANDLE_STMT, V_OD_hdbc, &V_OD_hstmt);

	SQLRETURN ret;
	ret = SQLExecDirect(V_OD_hstmt, (SQLCHAR*)"select * from \"tbATUHandOver\" where \"HOATT\" > 4 order by \"HOATT\" asc", SQL_NTS);
	int i = 0;
	V_OD_erg = SQLFetch(V_OD_hstmt);
	printf("%-20s%-20s%-10s\n", "SSECTOR_ID", "NSECTOR_ID", "HOATT");
	printf("------------------------------------------------------\n");
	while (V_OD_erg != SQL_NO_DATA)
	{
		SQLGetData(V_OD_hstmt, 1, SQL_C_DEFAULT, (SQLPOINTER)&SSECTOR_ID, 100, NULL);
		SQLGetData(V_OD_hstmt, 2, SQL_C_DEFAULT, (SQLPOINTER)&NSECTOR_ID, 100, NULL);
		SQLGetData(V_OD_hstmt, 3, SQL_C_DEFAULT, (SQLPOINTER)&HOATT, 100, NULL);
		printf("%-20s%-20s%-10d\n", SSECTOR_ID, NSECTOR_ID, HOATT);
		V_OD_erg = SQLFetch(V_OD_hstmt);
		i++;
	};

	// 断开数据源连接并释放句柄资源
	SQLFreeHandle(SQL_HANDLE_STMT, V_OD_hstmt);
	SQLDisconnect(V_OD_hdbc);
	SQLFreeHandle(SQL_HANDLE_DBC, V_OD_hdbc);
	SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);

	return (0);
}