/*
 * MIT License
 *
 * Copyright (c) 2022 freemine <freemine@yeah.net>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "odbc_helpers.h"

#include <stdarg.h>
#include <stdint.h>


#define DUMP(fmt, ...)          printf(fmt "\n", ##__VA_ARGS__)

typedef int (*dump_f)(SQLSMALLINT HandleType, SQLHANDLE Handle);

static int execute_sqls(SQLHANDLE hstmt, const char **sqls, size_t nr)
{
  SQLRETURN sr = SQL_SUCCESS;

  for (size_t i=0; i<nr; ++i) {
    const char *sql = sqls[i];
    sr = CALL_SQLExecDirect(hstmt, (SQLCHAR*)sql, SQL_NTS);
    if (sr != SQL_SUCCESS && sr != SQL_SUCCESS_WITH_INFO) return -1;
    CALL_SQLCloseCursor(hstmt);
  }

  return 0;
}

static int _dump_col_info(SQLHANDLE hstmt, SQLUSMALLINT ColumnNumber)
{
  SQLRETURN sr = SQL_SUCCESS;

  SQLUSMALLINT    FieldIdentifier;
  SQLPOINTER      CharacterAttributePtr;
  SQLSMALLINT     BufferLength;
  SQLSMALLINT     StringLength;
  SQLLEN          NumericAttribute;

  char buf[4096];

  FieldIdentifier             = SQL_DESC_AUTO_UNIQUE_VALUE;
  CharacterAttributePtr       = buf;
  BufferLength                = sizeof(buf);
  sr = CALL_SQLColAttribute(hstmt, ColumnNumber, FieldIdentifier, CharacterAttributePtr, BufferLength, &StringLength, &NumericAttribute);
  if (sr != SQL_SUCCESS) return -1;
  DUMP("Column%d.%s=%s",ColumnNumber, sql_col_attribute(FieldIdentifier), !!NumericAttribute ? "SQL_TRUE" : "SQL_FALSE");

  FieldIdentifier             = SQL_DESC_BASE_COLUMN_NAME;
  CharacterAttributePtr       = buf;
  BufferLength                = sizeof(buf);
  sr = CALL_SQLColAttribute(hstmt, ColumnNumber, FieldIdentifier, CharacterAttributePtr, BufferLength, &StringLength, &NumericAttribute);
  if (sr != SQL_SUCCESS) return -1;
  DUMP("Column%d.%s=%s",ColumnNumber, sql_col_attribute(FieldIdentifier), buf);

  FieldIdentifier             = SQL_DESC_BASE_TABLE_NAME;
  CharacterAttributePtr       = buf;
  BufferLength                = sizeof(buf);
  sr = CALL_SQLColAttribute(hstmt, ColumnNumber, FieldIdentifier, CharacterAttributePtr, BufferLength, &StringLength, &NumericAttribute);
  if (sr != SQL_SUCCESS) return -1;
  DUMP("Column%d.%s=%s",ColumnNumber, sql_col_attribute(FieldIdentifier), buf);

  FieldIdentifier             = SQL_DESC_CASE_SENSITIVE;
  CharacterAttributePtr       = buf;
  BufferLength                = sizeof(buf);
  sr = CALL_SQLColAttribute(hstmt, ColumnNumber, FieldIdentifier, CharacterAttributePtr, BufferLength, &StringLength, &NumericAttribute);
  if (sr != SQL_SUCCESS) return -1;
  DUMP("Column%d.%s=%s",ColumnNumber, sql_col_attribute(FieldIdentifier), !!NumericAttribute ? "SQL_TRUE" : "SQL_FALSE");

  FieldIdentifier             = SQL_DESC_CATALOG_NAME;
  CharacterAttributePtr       = buf;
  BufferLength                = sizeof(buf);
  sr = CALL_SQLColAttribute(hstmt, ColumnNumber, FieldIdentifier, CharacterAttributePtr, BufferLength, &StringLength, &NumericAttribute);
  if (sr != SQL_SUCCESS) return -1;
  DUMP("Column%d.%s=%s",ColumnNumber, sql_col_attribute(FieldIdentifier), buf);

  FieldIdentifier             = SQL_DESC_CONCISE_TYPE;
  CharacterAttributePtr       = buf;
  BufferLength                = sizeof(buf);
  sr = CALL_SQLColAttribute(hstmt, ColumnNumber, FieldIdentifier, CharacterAttributePtr, BufferLength, &StringLength, &NumericAttribute);
  if (sr != SQL_SUCCESS) return -1;
  DUMP("Column%d.%s=%s",ColumnNumber, sql_col_attribute(FieldIdentifier), sql_data_type((SQLSMALLINT)NumericAttribute));

  FieldIdentifier             = SQL_DESC_COUNT;
  CharacterAttributePtr       = buf;
  BufferLength                = sizeof(buf);
  sr = CALL_SQLColAttribute(hstmt, ColumnNumber, FieldIdentifier, CharacterAttributePtr, BufferLength, &StringLength, &NumericAttribute);
  if (sr != SQL_SUCCESS) return -1;
  DUMP("Column%d.%s=%" PRId64 "",ColumnNumber, sql_col_attribute(FieldIdentifier), (int64_t)NumericAttribute);

  FieldIdentifier             = SQL_DESC_DISPLAY_SIZE;
  CharacterAttributePtr       = buf;
  BufferLength                = sizeof(buf);
  sr = CALL_SQLColAttribute(hstmt, ColumnNumber, FieldIdentifier, CharacterAttributePtr, BufferLength, &StringLength, &NumericAttribute);
  if (sr != SQL_SUCCESS) return -1;
  DUMP("Column%d.%s=%" PRId64 "",ColumnNumber, sql_col_attribute(FieldIdentifier), (int64_t)NumericAttribute);

  FieldIdentifier             = SQL_DESC_FIXED_PREC_SCALE;
  CharacterAttributePtr       = buf;
  BufferLength                = sizeof(buf);
  sr = CALL_SQLColAttribute(hstmt, ColumnNumber, FieldIdentifier, CharacterAttributePtr, BufferLength, &StringLength, &NumericAttribute);
  if (sr != SQL_SUCCESS) return -1;
  DUMP("Column%d.%s=%" PRId64 "",ColumnNumber, sql_col_attribute(FieldIdentifier), (int64_t)NumericAttribute);

  FieldIdentifier             = SQL_DESC_LABEL;
  CharacterAttributePtr       = buf;
  BufferLength                = sizeof(buf);
  sr = CALL_SQLColAttribute(hstmt, ColumnNumber, FieldIdentifier, CharacterAttributePtr, BufferLength, &StringLength, &NumericAttribute);
  if (sr != SQL_SUCCESS) return -1;
  DUMP("Column%d.%s=%s",ColumnNumber, sql_col_attribute(FieldIdentifier), buf);

  FieldIdentifier             = SQL_DESC_LENGTH;
  CharacterAttributePtr       = buf;
  BufferLength                = sizeof(buf);
  sr = CALL_SQLColAttribute(hstmt, ColumnNumber, FieldIdentifier, CharacterAttributePtr, BufferLength, &StringLength, &NumericAttribute);
  if (sr != SQL_SUCCESS) return -1;
  DUMP("Column%d.%s=%" PRId64 "",ColumnNumber, sql_col_attribute(FieldIdentifier), (int64_t)NumericAttribute);

  FieldIdentifier             = SQL_DESC_LITERAL_PREFIX;
  CharacterAttributePtr       = buf;
  BufferLength                = sizeof(buf);
  sr = CALL_SQLColAttribute(hstmt, ColumnNumber, FieldIdentifier, CharacterAttributePtr, BufferLength, &StringLength, &NumericAttribute);
  if (sr != SQL_SUCCESS) return -1;
  DUMP("Column%d.%s=%s",ColumnNumber, sql_col_attribute(FieldIdentifier), buf);

  FieldIdentifier             = SQL_DESC_LITERAL_SUFFIX;
  CharacterAttributePtr       = buf;
  BufferLength                = sizeof(buf);
  sr = CALL_SQLColAttribute(hstmt, ColumnNumber, FieldIdentifier, CharacterAttributePtr, BufferLength, &StringLength, &NumericAttribute);
  if (sr != SQL_SUCCESS) return -1;
  DUMP("Column%d.%s=%s",ColumnNumber, sql_col_attribute(FieldIdentifier), buf);

  FieldIdentifier             = SQL_DESC_LOCAL_TYPE_NAME;
  CharacterAttributePtr       = buf;
  BufferLength                = sizeof(buf);
  sr = CALL_SQLColAttribute(hstmt, ColumnNumber, FieldIdentifier, CharacterAttributePtr, BufferLength, &StringLength, &NumericAttribute);
  if (sr != SQL_SUCCESS) return -1;
  DUMP("Column%d.%s=%s",ColumnNumber, sql_col_attribute(FieldIdentifier), buf);

  FieldIdentifier             = SQL_DESC_NAME;
  CharacterAttributePtr       = buf;
  BufferLength                = sizeof(buf);
  sr = CALL_SQLColAttribute(hstmt, ColumnNumber, FieldIdentifier, CharacterAttributePtr, BufferLength, &StringLength, &NumericAttribute);
  if (sr != SQL_SUCCESS) return -1;
  DUMP("Column%d.%s=%s",ColumnNumber, sql_col_attribute(FieldIdentifier), buf);

  FieldIdentifier             = SQL_DESC_NULLABLE;
  CharacterAttributePtr       = buf;
  BufferLength                = sizeof(buf);
  sr = CALL_SQLColAttribute(hstmt, ColumnNumber, FieldIdentifier, CharacterAttributePtr, BufferLength, &StringLength, &NumericAttribute);
  if (sr != SQL_SUCCESS) return -1;
  DUMP("Column%d.%s=%s",ColumnNumber, sql_col_attribute(FieldIdentifier), sql_nullable((SQLSMALLINT)NumericAttribute));

  FieldIdentifier             = SQL_DESC_NUM_PREC_RADIX;
  CharacterAttributePtr       = buf;
  BufferLength                = sizeof(buf);
  sr = CALL_SQLColAttribute(hstmt, ColumnNumber, FieldIdentifier, CharacterAttributePtr, BufferLength, &StringLength, &NumericAttribute);
  if (sr != SQL_SUCCESS) return -1;
  DUMP("Column%d.%s=%" PRId64 "",ColumnNumber, sql_col_attribute(FieldIdentifier), (int64_t)NumericAttribute);

  FieldIdentifier             = SQL_DESC_OCTET_LENGTH;
  CharacterAttributePtr       = buf;
  BufferLength                = sizeof(buf);
  sr = CALL_SQLColAttribute(hstmt, ColumnNumber, FieldIdentifier, CharacterAttributePtr, BufferLength, &StringLength, &NumericAttribute);
  if (sr != SQL_SUCCESS) return -1;
  DUMP("Column%d.%s=%" PRId64 "",ColumnNumber, sql_col_attribute(FieldIdentifier), (int64_t)NumericAttribute);

  FieldIdentifier             = SQL_DESC_PRECISION;
  CharacterAttributePtr       = buf;
  BufferLength                = sizeof(buf);
  sr = CALL_SQLColAttribute(hstmt, ColumnNumber, FieldIdentifier, CharacterAttributePtr, BufferLength, &StringLength, &NumericAttribute);
  if (sr != SQL_SUCCESS) return -1;
  DUMP("Column%d.%s=%" PRId64 "",ColumnNumber, sql_col_attribute(FieldIdentifier), (int64_t)NumericAttribute);

  FieldIdentifier             = SQL_DESC_SCALE;
  CharacterAttributePtr       = buf;
  BufferLength                = sizeof(buf);
  sr = CALL_SQLColAttribute(hstmt, ColumnNumber, FieldIdentifier, CharacterAttributePtr, BufferLength, &StringLength, &NumericAttribute);
  if (sr != SQL_SUCCESS) return -1;
  DUMP("Column%d.%s=%" PRId64 "",ColumnNumber, sql_col_attribute(FieldIdentifier), (int64_t)NumericAttribute);

  FieldIdentifier             = SQL_DESC_SCHEMA_NAME;
  CharacterAttributePtr       = buf;
  BufferLength                = sizeof(buf);
  sr = CALL_SQLColAttribute(hstmt, ColumnNumber, FieldIdentifier, CharacterAttributePtr, BufferLength, &StringLength, &NumericAttribute);
  if (sr != SQL_SUCCESS) return -1;
  DUMP("Column%d.%s=%s",ColumnNumber, sql_col_attribute(FieldIdentifier), buf);

  FieldIdentifier             = SQL_DESC_SEARCHABLE;
  CharacterAttributePtr       = buf;
  BufferLength                = sizeof(buf);
  sr = CALL_SQLColAttribute(hstmt, ColumnNumber, FieldIdentifier, CharacterAttributePtr, BufferLength, &StringLength, &NumericAttribute);
  if (sr != SQL_SUCCESS) return -1;
  DUMP("Column%d.%s=%s",ColumnNumber, sql_col_attribute(FieldIdentifier), sql_searchable((SQLSMALLINT)NumericAttribute));

  FieldIdentifier             = SQL_DESC_TABLE_NAME;
  CharacterAttributePtr       = buf;
  BufferLength                = sizeof(buf);
  sr = CALL_SQLColAttribute(hstmt, ColumnNumber, FieldIdentifier, CharacterAttributePtr, BufferLength, &StringLength, &NumericAttribute);
  if (sr != SQL_SUCCESS) return -1;
  DUMP("Column%d.%s=%s",ColumnNumber, sql_col_attribute(FieldIdentifier), buf);

  FieldIdentifier             = SQL_DESC_TYPE;
  CharacterAttributePtr       = buf;
  BufferLength                = sizeof(buf);
  sr = CALL_SQLColAttribute(hstmt, ColumnNumber, FieldIdentifier, CharacterAttributePtr, BufferLength, &StringLength, &NumericAttribute);
  if (sr != SQL_SUCCESS) return -1;
  DUMP("Column%d.%s=%s",ColumnNumber, sql_col_attribute(FieldIdentifier), sql_data_type((SQLSMALLINT)NumericAttribute));

  FieldIdentifier             = SQL_DESC_TYPE_NAME;
  CharacterAttributePtr       = buf;
  BufferLength                = sizeof(buf);
  sr = CALL_SQLColAttribute(hstmt, ColumnNumber, FieldIdentifier, CharacterAttributePtr, BufferLength, &StringLength, &NumericAttribute);
  if (sr != SQL_SUCCESS) return -1;
  DUMP("Column%d.%s=%s",ColumnNumber, sql_col_attribute(FieldIdentifier), buf);

  FieldIdentifier             = SQL_DESC_UNNAMED;
  CharacterAttributePtr       = buf;
  BufferLength                = sizeof(buf);
  sr = CALL_SQLColAttribute(hstmt, ColumnNumber, FieldIdentifier, CharacterAttributePtr, BufferLength, &StringLength, &NumericAttribute);
  if (sr != SQL_SUCCESS) return -1;
  DUMP("Column%d.%s=%" PRId64 "",ColumnNumber, sql_col_attribute(FieldIdentifier), (int64_t)NumericAttribute);

  FieldIdentifier             = SQL_DESC_UNSIGNED;
  CharacterAttributePtr       = buf;
  BufferLength                = sizeof(buf);
  sr = CALL_SQLColAttribute(hstmt, ColumnNumber, FieldIdentifier, CharacterAttributePtr, BufferLength, &StringLength, &NumericAttribute);
  if (sr != SQL_SUCCESS) return -1;
  DUMP("Column%d.%s=%s",ColumnNumber, sql_col_attribute(FieldIdentifier), !!NumericAttribute ? "SQL_TRUE" : "SQL_FALSE");

  FieldIdentifier             = SQL_DESC_UPDATABLE;
  CharacterAttributePtr       = buf;
  BufferLength                = sizeof(buf);
  sr = CALL_SQLColAttribute(hstmt, ColumnNumber, FieldIdentifier, CharacterAttributePtr, BufferLength, &StringLength, &NumericAttribute);
  if (sr != SQL_SUCCESS) return -1;
  DUMP("Column%d.%s=%s",ColumnNumber, sql_col_attribute(FieldIdentifier), sql_updatable((SQLSMALLINT)NumericAttribute));

  return 0;
}

static int _dump_stmt_col_info(SQLSMALLINT HandleType, SQLHANDLE Handle)
{
  SQLRETURN sr = SQL_SUCCESS;
  int r = 0;

  if (HandleType != SQL_HANDLE_STMT) return 0;

  DUMP("");
  DUMP("%s:", __func__);

  SQLHANDLE hstmt = Handle;

  const char *sqls[] = {
    "drop table if exists x",
    "create table x (ts timestamp, b bit, i8 tinyint, i16 smallint, i32 int, i64 bigint, f real, d float, name varchar(20), mark nchar(2), dt datetime, dt2 datetime2(3))",
  };

  r = execute_sqls(hstmt, sqls, sizeof(sqls)/sizeof(sqls[0]));
  if (r) return -1;

  const char *sql = "select * from x";
  sr = CALL_SQLExecDirect(hstmt, (SQLCHAR*)sql, SQL_NTS);
  if (sr != SQL_SUCCESS) return -1;

  SQLSMALLINT ColumnCount;
  sr = CALL_SQLNumResultCols(hstmt, &ColumnCount);
  if (sr != SQL_SUCCESS) return -1;

  for (SQLSMALLINT i = 0; i<ColumnCount; ++i) {
    r = _dump_col_info(hstmt, (SQLUSMALLINT)i+1);
    if (r) return -1;
  }

  return 0;
}

static int _dump_col(SQLHANDLE hstmt, SQLUSMALLINT ColumnNumber)
{
  SQLRETURN sr = SQL_SUCCESS;

  char buf[4096];

  SQLUSMALLINT   Col_or_Param_Num = ColumnNumber;
  SQLSMALLINT    TargetType       = SQL_C_CHAR;
  SQLPOINTER     TargetValuePtr   = buf;
  SQLLEN         BufferLength     = sizeof(buf);
  SQLLEN         StrLen_or_Ind;

  sr = CALL_SQLGetData(hstmt, Col_or_Param_Num, TargetType, TargetValuePtr, BufferLength, &StrLen_or_Ind);
  if (sr != SQL_SUCCESS) return -1;
  if (StrLen_or_Ind == SQL_NULL_DATA) {
    DUMP("Column%d:[null]", Col_or_Param_Num);
  } else {
    DUMP("Column%d:%.*s", Col_or_Param_Num, (int)StrLen_or_Ind, buf);
  }

  return 0;
}

static int _dump_stmt_tables(SQLSMALLINT HandleType, SQLHANDLE Handle)
{
  SQLRETURN sr = SQL_SUCCESS;
  int r = 0;

  if (HandleType != SQL_HANDLE_STMT) return 0;

  int row = 0;

  DUMP("");
  DUMP("%s:", __func__);

  SQLHANDLE hstmt = Handle;
  const char *CatalogName;
  const char *SchemaName;
  const char *TableName;
  const char *TableType;

  CatalogName = "information_schema";
  SchemaName = NULL;
  TableName = "ins_columns";
  TableType = "TABLE";
  sr = CALL_SQLTables(hstmt,
    (SQLCHAR*)CatalogName, (SQLSMALLINT)strlen(CatalogName),
    (SQLCHAR*)SchemaName,  SchemaName ? (SQLSMALLINT)strlen(SchemaName) : 0,
    (SQLCHAR*)TableName,   (SQLSMALLINT)strlen(TableName),
    (SQLCHAR*)TableType,   (SQLSMALLINT)strlen(TableType));
  DUMP("sr: %s", sql_return_type(sr));
  if (FAILED(sr)) return -1;

again:
  sr = CALL_SQLFetch(hstmt);
  if (sr == SQL_NO_DATA) return 0;
  if (sr != SQL_SUCCESS) return -1;

  ++row;
  DUMP("row:%d", row);

  SQLSMALLINT ColumnCount;
  sr = CALL_SQLNumResultCols(hstmt, &ColumnCount);
  if (sr != SQL_SUCCESS) return -1;

  for (SQLSMALLINT i = 0; i<ColumnCount; ++i) {
    r = _dump_col(hstmt, (SQLUSMALLINT)i+1);
    if (r) return -1;
  }

  goto again;

  return 0;
}

static int _dump_stmt_describe_col(SQLSMALLINT HandleType, SQLHANDLE Handle)
{
  SQLRETURN sr = SQL_SUCCESS;
  int r = 0;

  if (HandleType != SQL_HANDLE_STMT) return 0;

  DUMP("");
  DUMP("%s:", __func__);

  SQLHANDLE hstmt = Handle;

  const char *sqls[] = {
    "drop table if exists x",
    "create table x (ts timestamp, b bit, i8 tinyint, i16 smallint, i32 int, i64 bigint, f real, d float, name varchar(20), mark nchar(2), dt datetime, dt2 datetime2(3))",
  };

  r = execute_sqls(hstmt, sqls, sizeof(sqls)/sizeof(sqls[0]));
  if (r) return -1;

  const char *sql = "select * from x";
  sr = CALL_SQLExecDirect(hstmt, (SQLCHAR*)sql, SQL_NTS);
  if (sr != SQL_SUCCESS) return -1;

  SQLSMALLINT ColumnCount;
  sr = CALL_SQLNumResultCols(hstmt, &ColumnCount);
  if (sr != SQL_SUCCESS) return -1;

  for (SQLSMALLINT i = 0; i<ColumnCount; ++i) {
    char buf[4096];
    SQLSMALLINT    NameLength;
    SQLSMALLINT    DataType;
    SQLULEN        ColumnSize;
    SQLSMALLINT    DecimalDigits;
    SQLSMALLINT    Nullable;
    SQLLEN         NumericAttribute;

    sr = CALL_SQLDescribeCol(hstmt, i+1, (SQLCHAR*)buf, sizeof(buf), &NameLength, &DataType, &ColumnSize, &DecimalDigits, &Nullable);
    if (sr != SQL_SUCCESS) return -1;
    DUMP("Column%d Name:%s, DataType:%s, ColumnSize:%" PRIu64 ", DecimalDigits:%d, Nullable:%s",
      i+1, buf, sql_data_type(DataType), (uint64_t)ColumnSize, DecimalDigits, sql_nullable(Nullable));

    sr = CALL_SQLColAttribute(hstmt, i+1, SQL_DESC_NAME, (SQLCHAR*)buf, sizeof(buf), &NameLength, &NumericAttribute);
    if (sr != SQL_SUCCESS) return -1;

    SQLLEN         DESC_LENGTH;
    sr = CALL_SQLColAttribute(hstmt, i+1, SQL_DESC_LENGTH, NULL, 0, NULL, &DESC_LENGTH);
    if (sr != SQL_SUCCESS) return -1;

    SQLLEN         DESC_PRECISION;
    sr = CALL_SQLColAttribute(hstmt, i+1, SQL_DESC_PRECISION, NULL, 0, NULL, &DESC_PRECISION);
    if (sr != SQL_SUCCESS) return -1;

    SQLLEN         DESC_SCALE;
    sr = CALL_SQLColAttribute(hstmt, i+1, SQL_DESC_SCALE, NULL, 0, NULL, &DESC_SCALE);
    if (sr != SQL_SUCCESS) return -1;

    SQLLEN         DESC_OCTET_LENGTH;
    sr = CALL_SQLColAttribute(hstmt, i+1, SQL_DESC_OCTET_LENGTH, NULL, 0, NULL, &DESC_OCTET_LENGTH);
    if (sr != SQL_SUCCESS) return -1;

    DUMP("Column%d Name:%s, DESC_LENGTH:%" PRId64 ", DESC_PRECISION:%" PRId64 ", DESC_SCALE:%" PRId64 ", DESC_OCTET_LENGTH:%" PRId64 "",
      i+1, buf, (int64_t)DESC_LENGTH, (int64_t)DESC_PRECISION, (int64_t)DESC_SCALE, (int64_t)DESC_OCTET_LENGTH);
  }

  return 0;
}

#define RECORD(x) {x, #x}

static struct {
  dump_f                func;
  const char           *name;
} _dumps[] = {
  RECORD(_dump_stmt_col_info),
  RECORD(_dump_stmt_tables),
  RECORD(_dump_stmt_describe_col),
};

typedef struct arg_s             arg_t;
struct arg_s {
  const char      *dsn;
  const char      *uid;
  const char      *pwd;
  const char      *connstr;

  const char      *name;
};

static int _connect(SQLHANDLE hconn, const char *dsn, const char *uid, const char *pwd)
{
  SQLRETURN sr = SQL_SUCCESS;

  sr = CALL_SQLConnect(hconn, (SQLCHAR*)dsn, SQL_NTS, (SQLCHAR*)uid, SQL_NTS, (SQLCHAR*)pwd, SQL_NTS);
  if (FAILED(sr)) {
    E("connect [dsn:%s,uid:%s,pwd:%s] failed", dsn, uid, pwd);
    return -1;
  }

  return 0;
}

static int _driver_connect(SQLHANDLE hconn, const char *connstr)
{
  SQLRETURN sr = SQL_SUCCESS;

  sr = CALL_SQLDriverConnect(hconn, NULL, (SQLCHAR*)connstr, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);
  if (FAILED(sr)) {
    E("driver_connect [connstr:%s] failed", connstr);
    return -1;
  }

  return 0;
}

static int dump_with_stmt(const arg_t *arg, SQLHANDLE hstmt)
{
  int r = 0;

  const char *sqls[] = {
    "drop database if exists bar",
    "create database bar",
    "use bar",
  };

  r = execute_sqls(hstmt, sqls, sizeof(sqls)/sizeof(sqls[0]));
  if (r) return -1;

  for (size_t i=0; i<sizeof(_dumps)/sizeof(_dumps[0]); ++i) {
    if (arg->name == NULL || strcmp(arg->name, _dumps[i].name)==0) {
      r = _dumps[i].func(SQL_HANDLE_STMT, hstmt);
      if (r) return -1;
    }
  }

  return 0;
}

static int dump_with_connected_conn(const arg_t *arg, SQLHANDLE hconn)
{
  SQLRETURN sr = SQL_SUCCESS;
  int r = 0;

  for (size_t i=0; i<sizeof(_dumps)/sizeof(_dumps[0]); ++i) {
    if (arg->name == NULL || strcmp(arg->name, _dumps[i].name)==0) {
      r = _dumps[i].func(SQL_HANDLE_DBC, hconn);
      if (r) return -1;
    }
  }

  SQLHANDLE hstmt;

  sr = CALL_SQLAllocHandle(SQL_HANDLE_STMT, hconn, &hstmt);
  if (sr != SQL_SUCCESS) return SQL_ERROR;

  r = dump_with_stmt(arg, hstmt);

  CALL_SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

  return r ? -1 : 0;
}

static int dump_with_conn(const arg_t *arg, SQLHANDLE hconn)
{
  int r = 0;

  if (arg->connstr) {
    r = _driver_connect(hconn, arg->connstr);
    if (r) return -1;
  } else {
    r = _connect(hconn, arg->dsn, arg->uid, arg->pwd);
    if (r) return -1;
  }

  r = dump_with_connected_conn(arg, hconn);

  CALL_SQLDisconnect(hconn);

  return r ? -1 : 0;
}

static int dump_with_env(const arg_t *arg, SQLHANDLE henv)
{
  int r = 0;
  SQLRETURN sr = SQL_SUCCESS;

  SQLHANDLE hconn = SQL_NULL_HANDLE;
  sr = CALL_SQLAllocHandle(SQL_HANDLE_DBC, henv, &hconn);
  if (FAILED(sr)) return -1;

  r = dump_with_conn(arg, hconn);

  CALL_SQLFreeHandle(SQL_HANDLE_DBC, hconn);

  return r;
}

static int _dumping(const arg_t *arg)
{
  int r = 0;
  SQLRETURN sr = SQL_SUCCESS;

  SQLHANDLE henv  = SQL_NULL_HANDLE;

  sr = CALL_SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
  if (FAILED(sr)) return 1;

  do {
    sr = CALL_SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
    if (FAILED(sr)) { r = -1; break; }

    r = dump_with_env(arg, henv);
    if (r) break;
  } while (0);

  CALL_SQLFreeHandle(SQL_HANDLE_ENV, henv);

  return r;
}

static void usage(const char *arg0)
{
  DUMP("usage:");
  DUMP("  %s -h", arg0);
  DUMP("  %s --dsn <DSN> [--uid <UID>] [--pwd <PWD> [name]", arg0);
  DUMP("  %s --connstr <connstr> [name]", arg0);
  DUMP("");
  DUMP("supported dump names:");
  for (size_t i=0; i<sizeof(_dumps)/sizeof(_dumps[0]); ++i) {
    DUMP("  %s", _dumps[i].name);
  }
}

static int dumping(int argc, char *argv[])
{
  int r = 0;

  arg_t arg = {0};

  for (int i=1; i<argc; ++i) {
    if (strcmp(argv[i], "-h") == 0) {
      usage(argv[0]);
      return 0;
    }
    if (strcmp(argv[i], "--dsn") == 0) {
      ++i;
      if (i>=argc) break;
      arg.dsn = argv[i];
      continue;
    }
    if (strcmp(argv[i], "--uid") == 0) {
      ++i;
      if (i>=argc) break;
      arg.uid = argv[i];
      continue;
    }
    if (strcmp(argv[i], "--pwd") == 0) {
      ++i;
      if (i>=argc) break;
      arg.pwd = argv[i];
      continue;
    }
    if (strcmp(argv[i], "--connstr") == 0) {
      ++i;
      if (i>=argc) break;
      arg.connstr = argv[i];
      continue;
    }

    arg.name = argv[i];
    r = _dumping(&arg);
    if (r) return -11;
  }

  if (arg.name) return 0;

  return _dumping(&arg);
}

int main(int argc, char *argv[])
{
  int r = 0;
  r = dumping(argc, argv);
  fprintf(stderr, "==%s==\n", r ? "failure" : "success");
  return !!r;
}