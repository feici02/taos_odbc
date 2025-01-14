/*
 * MIT License
 *
 * Copyright (c) 2022-2023 freemine <freemine@yeah.net>
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

%code top {
}

%code top {
    // here to include header files required for generated code
}

%code requires {
    #define YYSTYPE       EXT_PARSER_YYSTYPE
    #define YYLTYPE       EXT_PARSER_YYLTYPE
    #ifndef YY_TYPEDEF_YY_SCANNER_T
    #define YY_TYPEDEF_YY_SCANNER_T
    typedef void* yyscan_t;
    #endif
}

%code provides {
}

%code {
    // generated header from flex
    // introduce yylex decl for later use
    static void _yyerror_impl(
        YYLTYPE *yylloc,                   // match %define locations
        yyscan_t arg,                      // match %param
        ext_parser_param_t *param,         // match %parse-param
        const char *errmsg
    );
    static void yyerror(
        YYLTYPE *yylloc,                   // match %define locations
        yyscan_t arg,                      // match %param
        ext_parser_param_t *param,         // match %parse-param
        const char *errsg
    );

    static int ext_parser_param_append_topic_name(ext_parser_param_t *param, const char *name, size_t len);
    static int ext_parser_param_append_topic_conf(ext_parser_param_t *param, const char *k, size_t kn, const char *v, size_t vn);

    #define SET_TOPIC(_v, _loc) do {                                                            \
      if (!param) break;                                                                        \
      if (ext_parser_param_append_topic_name(param, _v.text, _v.leng)) {                               \
        _yyerror_impl(&_loc, arg, param, "runtime error:out of memory");                        \
        return -1;                                                                              \
      }                                                                                         \
    } while (0)

    #define SET_TOPIC_KEY(_k, _loc) do {                                                        \
      if (!param) break;                                                                        \
      if (ext_parser_param_append_topic_conf(param, _k.text, _k.leng, NULL, 0)) {                   \
        _yyerror_impl(&_loc, arg, param, "runtime error:out of memory");                        \
        return -1;                                                                              \
      }                                                                                         \
    } while (0)

    #define SET_TOPIC_KEY_VAL(_k, _v, _loc) do {                                                \
      if (!param) break;                                                                        \
      if (ext_parser_param_append_topic_conf(param, _k.text, _k.leng, _v.text, _v.leng)) {          \
        _yyerror_impl(&_loc, arg, param, "runtime error:out of memory");                        \
        return -1;                                                                              \
      }                                                                                         \
    } while (0)

    void ext_parser_param_release(ext_parser_param_t *param)
    {
      if (!param) return;
      topic_cfg_release(&param->topic_cfg);
      param->ctx.err_msg[0] = '\0';
      param->ctx.row0 = 0;
    }
}

/* Bison declarations. */
%require "3.0.4"
%define api.pure full
%define api.token.prefix {TOK_}
%define locations
%define parse.error verbose
%define parse.lac full
%define parse.trace true
%defines
%verbose

%param { yyscan_t arg }
%parse-param { ext_parser_param_t *param }

// union members
%union { parser_token_t token; }
%union { char c; }

%token TOPIC
%token <token> DIGITS
%token <token> TNAME TKEY TVAL

 /* %nterm <str>   args */ // non-terminal `input` use `str` to store
                           // token value as well
 /* %destructor { free($$); } <str> */

%% /* The grammar follows. */

input:
  %empty
| topic                     { (void)yynerrs; }
;

topic:
  '!' TOPIC names
| '!' TOPIC names '{' tconfs '}'
;

names:
  TNAME                     { SET_TOPIC($1, @$); }
| names TNAME               { SET_TOPIC($2, @$); }
;

tconfs:
  %empty
| tconf
| tconfs delimits tconf
;

tconf:
  TKEY                           { SET_TOPIC_KEY($1, @$); }
| TKEY '=' TVAL                  { SET_TOPIC_KEY_VAL($1, $3, @$); }
;

delimits:
  ';'
| delimits ';'
;

%%

static void _yyerror_impl(
    YYLTYPE *yylloc,                   // match %define locations
    yyscan_t arg,                      // match %param
    ext_parser_param_t *param,         // match %parse-param
    const char *errmsg
)
{
  // to implement it here
  (void)yylloc;
  (void)arg;
  (void)param;
  (void)errmsg;

  if (!param) {
    fprintf(stderr, "(%d,%d)->(%d,%d):%s\n",
        yylloc->first_line, yylloc->first_column,
        yylloc->last_line, yylloc->last_column,
        errmsg);

    return;
  }

  param->ctx.row0 = yylloc->first_line;
  param->ctx.col0 = yylloc->first_column;
  param->ctx.row1 = yylloc->last_line;
  param->ctx.col1 = yylloc->last_column;
  param->ctx.err_msg[0] = '\0';
  snprintf(param->ctx.err_msg, sizeof(param->ctx.err_msg), "%s", errmsg);
}

/* Called by yyparse on error. */
static void yyerror(
    YYLTYPE *yylloc,                   // match %define locations
    yyscan_t arg,                      // match %param
    ext_parser_param_t *param,         // match %parse-param
    const char *errmsg
)
{
  _yyerror_impl(yylloc, arg, param, errmsg);
}

static int ext_parser_param_append_topic_name(ext_parser_param_t *param, const char *name, size_t len)
{
  topic_cfg_t *cfg = &param->topic_cfg;
  if (cfg->names_nr == cfg->names_cap) {
    size_t cap = cfg->names_cap + 16;
    char **names = (char**)realloc(cfg->names, sizeof(*names) * cap);
    if (!names) return -1;
    cfg->names = names;
    cfg->names_cap = cap;
  }
  cfg->names[cfg->names_nr] = strndup(name, len);
  if (!cfg->names[cfg->names_nr]) return -1;
  cfg->names_nr += 1;
  return 0;
}

static int ext_parser_param_append_topic_conf(ext_parser_param_t *param, const char *k, size_t kn, const char *v, size_t vn)
{
  topic_cfg_t *cfg = &param->topic_cfg;
  return topic_cfg_append_kv(cfg, k, kn, v, vn);
}

int ext_parser_parse(const char *input, size_t len, ext_parser_param_t *param)
{
  yyscan_t arg = {0};
  yylex_init(&arg);
  // yyset_in(in, arg);
  int debug_flex = param ? param->ctx.debug_flex : 0;
  int debug_bison = param ? param->ctx.debug_bison: 0;
  yyset_debug(debug_flex, arg);
  yydebug = debug_bison;
  // yyset_extra(param, arg);
  yy_scan_bytes(input ? input : "", input ? (int)len : 0, arg);
  int ret =yyparse(arg, param);
  yylex_destroy(arg);
  return ret ? -1 : 0;
}

