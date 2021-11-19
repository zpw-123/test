/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 2 "yacc_sql.y"


#include "sql/parser/parse_defs.h"
#include "sql/parser/yacc_sql.tab.h"
#include "sql/parser/lex.yy.h"
// #include "common/log/log.h" // 包含C++中的头文件

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct ParserContext {
  Query * ssql;
  size_t select_length;
  size_t condition_length;
  size_t from_length;
  size_t value_length;
  Value values[MAX_NUM];
  Condition conditions[MAX_NUM];
  CompOp comp;
  char id[MAX_NUM];
  size_t data_num;
} ParserContext;

//获取子串
char *substr(const char *s,int n1,int n2)/*从s中提取下标为n1~n2的字符组成一个新字符串，然后返回这个新串的首地址*/
{
  char *sp = malloc(sizeof(char) * (n2 - n1 + 2));
  int i, j = 0;
  for (i = n1; i <= n2; i++) {
    sp[j++] = s[i];
  }
  sp[j] = 0;
  return sp;
}

void yyerror(yyscan_t scanner, const char *str)
{
  ParserContext *context = (ParserContext *)(yyget_extra(scanner));
  query_reset(context->ssql);
  context->ssql->flag = SCF_ERROR;
  context->condition_length = 0;
  context->from_length = 0;
  context->select_length = 0;
  context->value_length = 0;
  context->ssql->sstr.insertion.value_num = 0;
  context->data_num = 0;
  printf("parse sql failed. error=%s", str);
}

ParserContext *get_context(yyscan_t scanner)
{
  return (ParserContext *)yyget_extra(scanner);
}

#define CONTEXT get_context(scanner)


#line 129 "yacc_sql.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_YACC_SQL_TAB_H_INCLUDED
# define YY_YY_YACC_SQL_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    SEMICOLON = 258,
    CREATE = 259,
    DROP = 260,
    TABLE = 261,
    TABLES = 262,
    INDEX = 263,
    SELECT = 264,
    ASC = 265,
    DESC = 266,
    SHOW = 267,
    SYNC = 268,
    INSERT = 269,
    DELETE = 270,
    UPDATE = 271,
    LBRACE = 272,
    RBRACE = 273,
    COMMA = 274,
    TRX_BEGIN = 275,
    TRX_COMMIT = 276,
    TRX_ROLLBACK = 277,
    INT_T = 278,
    STRING_T = 279,
    FLOAT_T = 280,
    DATE_T = 281,
    HELP = 282,
    EXIT = 283,
    DOT = 284,
    INTO = 285,
    VALUES = 286,
    FROM = 287,
    WHERE = 288,
    AND = 289,
    SET = 290,
    ON = 291,
    LOAD = 292,
    DATA = 293,
    INFILE = 294,
    EQ = 295,
    LT = 296,
    GT = 297,
    LE = 298,
    GE = 299,
    NE = 300,
    INNER = 301,
    JOIN = 302,
    ORDER = 303,
    GROUP = 304,
    BY = 305,
    UNIQUE = 306,
    TEXT_T = 307,
    NOT = 308,
    NULL_T = 309,
    NULLABLE = 310,
    IS_T = 311,
    NUMBER = 312,
    FLOAT = 313,
    ID = 314,
    PATH = 315,
    SSS = 316,
    STAR = 317,
    STRING_V = 318,
    MAX = 319,
    MIN = 320,
    COUNT = 321,
    AVG = 322
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 121 "yacc_sql.y"

  struct _Attr *attr;
  struct _Condition *condition1;
  struct _Value *value1;
  char *string;
  int number;
  float floats;
	char *position;

#line 259 "yacc_sql.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int yyparse (void *scanner);

#endif /* !YY_YY_YACC_SQL_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   348

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  68
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  41
/* YYNRULES -- Number of rules.  */
#define YYNRULES  139
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  334

#define YYUNDEFTOK  2
#define YYMAXUTOK   322


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   152,   152,   154,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   178,   183,   188,   194,   200,   206,   212,   218,   224,
     231,   236,   244,   251,   260,   262,   266,   283,   299,   308,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   322,   325,   334,   351,   353,   358,   363,   365,   370,
     373,   376,   380,   387,   397,   407,   423,   439,   442,   447,
     452,   457,   462,   467,   477,   487,   497,   507,   517,   527,
     536,   545,   557,   559,   566,   573,   580,   590,   600,   610,
     620,   630,   635,   645,   734,   736,   740,   742,   747,   749,
     756,   763,   770,   777,   784,   793,   795,   802,   809,   816,
     823,   830,   839,   841,   848,   857,   859,   866,   876,   878,
     883,   904,   924,   944,   966,   987,  1008,  1027,  1036,  1045,
    1054,  1063,  1073,  1086,  1087,  1088,  1089,  1090,  1091,  1095
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "SEMICOLON", "CREATE", "DROP", "TABLE",
  "TABLES", "INDEX", "SELECT", "ASC", "DESC", "SHOW", "SYNC", "INSERT",
  "DELETE", "UPDATE", "LBRACE", "RBRACE", "COMMA", "TRX_BEGIN",
  "TRX_COMMIT", "TRX_ROLLBACK", "INT_T", "STRING_T", "FLOAT_T", "DATE_T",
  "HELP", "EXIT", "DOT", "INTO", "VALUES", "FROM", "WHERE", "AND", "SET",
  "ON", "LOAD", "DATA", "INFILE", "EQ", "LT", "GT", "LE", "GE", "NE",
  "INNER", "JOIN", "ORDER", "GROUP", "BY", "UNIQUE", "TEXT_T", "NOT",
  "NULL_T", "NULLABLE", "IS_T", "NUMBER", "FLOAT", "ID", "PATH", "SSS",
  "STAR", "STRING_V", "MAX", "MIN", "COUNT", "AVG", "$accept", "commands",
  "command", "exit", "help", "sync", "begin", "commit", "rollback",
  "drop_table", "show_tables", "desc_table", "create_index", "drop_index",
  "create_table", "attr_def_list", "attr_def", "number", "type", "ID_get",
  "insert", "muti_value_list", "muti_value", "value_list", "value",
  "delete", "update", "select", "join_list", "select_attr", "attr_list",
  "rel_list", "where", "order_by", "order_by_list", "group_by",
  "group_by_list", "condition_list", "condition", "comOp", "load_data", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322
};
# endif

#define YYPACT_NINF (-299)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -299,    60,  -299,     6,    15,    95,   -51,    39,    51,    28,
     -10,     3,    80,   110,   116,   129,   142,   101,  -299,  -299,
    -299,  -299,  -299,  -299,  -299,  -299,  -299,  -299,  -299,  -299,
    -299,  -299,  -299,  -299,  -299,  -299,    97,   104,   175,   128,
     137,    20,  -299,   178,   185,   191,   195,   182,   212,   213,
    -299,   158,   159,   184,  -299,  -299,  -299,  -299,  -299,   181,
     204,   186,   164,   221,   222,   113,    36,  -299,   167,   168,
      40,   169,   170,  -299,  -299,   199,   198,   173,   172,   176,
     177,   201,  -299,  -299,    67,   217,   223,   224,   225,   219,
     219,    12,    18,    19,   226,    27,   -12,   228,   127,   236,
     203,   216,  -299,   229,    -8,   230,   190,    52,  -299,   192,
     193,    53,   194,  -299,  -299,   219,   196,   219,   197,   219,
     200,  -299,   219,   202,   205,   207,   198,   198,    79,   231,
    -299,  -299,  -299,   -16,  -299,    65,   232,  -299,    79,   251,
     176,   240,    91,   100,   120,   121,  -299,   243,   206,   245,
     219,   219,    34,    41,   249,   250,    71,  -299,   252,  -299,
     253,  -299,   254,  -299,   255,   244,   210,   271,   227,   257,
     228,   274,   220,  -299,  -299,  -299,  -299,  -299,  -299,    25,
     140,    96,   146,   127,  -299,   198,   233,   229,   275,   234,
    -299,   235,  -299,   237,  -299,   239,  -299,   238,   262,   241,
    -299,  -299,   219,   242,   219,   246,   219,   219,   219,   247,
     219,   219,   219,   219,  -299,   248,  -299,   258,   260,    79,
     263,   231,  -299,   126,   256,  -299,  -299,  -299,   259,  -299,
     261,  -299,   232,   279,   280,  -299,  -299,  -299,  -299,  -299,
    -299,  -299,   267,   283,   269,  -299,   276,  -299,   278,  -299,
    -299,  -299,   281,  -299,  -299,  -299,  -299,   127,   264,   265,
     294,   257,  -299,  -299,   136,   152,  -299,  -299,   266,  -299,
    -299,  -299,  -299,  -299,   295,   219,   219,   219,   232,    32,
     268,  -299,  -299,   270,  -299,   273,  -299,  -299,  -299,  -299,
    -299,  -299,   272,   284,   284,   277,   282,  -299,    72,  -299,
     285,  -299,  -299,  -299,    74,   106,   286,   287,  -299,  -299,
     284,   284,   288,  -299,   284,   284,  -299,    75,   292,  -299,
    -299,   124,  -299,  -299,   289,  -299,  -299,   284,   284,  -299,
     292,  -299,  -299,  -299
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     3,    20,
      19,    14,    15,    16,    17,     9,    10,    11,    12,    13,
       8,     5,     7,     6,     4,    18,     0,     0,     0,     0,
       0,    82,    69,     0,     0,     0,     0,     0,     0,     0,
      23,     0,     0,     0,    24,    25,    26,    22,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    70,     0,     0,
       0,     0,     0,    29,    28,     0,    96,     0,     0,     0,
       0,     0,    27,    32,    82,     0,     0,     0,     0,    82,
      82,     0,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,    52,    34,     0,     0,     0,     0,    83,     0,
       0,     0,     0,    71,    72,    82,     0,    82,     0,    82,
       0,    79,    82,     0,     0,     0,    96,    96,     0,    54,
      62,    59,    60,     0,    61,     0,   118,    63,     0,     0,
       0,     0,    40,    43,    46,    49,    38,    37,     0,     0,
      82,    82,     0,     0,     0,     0,     0,    73,     0,    75,
       0,    77,     0,    80,     0,    94,     0,     0,    98,    57,
       0,     0,     0,   133,   134,   135,   136,   137,   138,     0,
       0,     0,     0,     0,    97,    96,     0,    34,     0,     0,
      42,     0,    45,     0,    48,     0,    51,     0,     0,     0,
      84,    85,    82,     0,    82,     0,    82,    82,    82,     0,
      82,    82,    82,    82,    95,     0,    66,     0,   112,     0,
       0,    54,    53,     0,     0,   127,   122,   120,     0,   131,
     123,   121,   118,     0,     0,    35,    33,    41,    44,    47,
      50,    39,     0,     0,     0,    86,     0,    88,     0,    90,
      91,    92,     0,    74,    76,    78,    81,     0,     0,     0,
       0,    57,    56,    55,     0,     0,   128,   132,     0,   119,
      64,   139,    36,    30,     0,    82,    82,    82,   118,   105,
       0,    65,    58,     0,   129,     0,   124,   125,    31,    87,
      89,    93,    67,   105,   105,     0,     0,    99,   115,   130,
       0,    68,   100,   101,   105,   105,     0,     0,   113,   126,
     105,   105,     0,   106,   105,   105,   102,   115,   115,   107,
     108,   105,   103,   104,     0,   116,   114,   105,   105,   109,
     115,   110,   111,   117
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -299,  -299,  -299,  -299,  -299,  -299,  -299,  -299,  -299,  -299,
    -299,  -299,  -299,  -299,  -299,   117,   174,  -299,  -299,  -299,
    -299,    86,   147,    55,  -127,  -299,  -299,  -299,    29,  -299,
     -84,   154,  -117,  -299,  -163,  -299,  -298,  -228,  -180,  -133,
    -299
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,   141,   103,   242,   147,   104,
      31,   171,   129,   220,   135,    32,    33,    34,   126,    47,
      67,   127,    99,   218,   297,   260,   308,   184,   136,   180,
      35
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     108,   169,   182,   232,   269,   113,   114,   124,    48,   167,
     168,   185,    36,   172,    37,   142,   143,   144,   145,   325,
     326,    39,    52,    40,   173,   174,   175,   176,   177,   178,
     115,   157,   333,   159,   125,   161,   117,   119,   163,    65,
     179,   116,   293,   294,   146,   122,    49,   118,   120,    66,
     292,   295,   202,   227,    50,   231,   123,    38,    51,   204,
       2,   296,    53,   203,     3,     4,   200,   201,   233,     5,
     205,     6,     7,     8,     9,    10,    11,   278,   224,   225,
      12,    13,    14,    54,   310,   311,    65,    15,    16,   208,
     265,   306,   261,   295,   306,    89,   107,    17,    90,    93,
     209,   307,    94,   312,   324,   173,   174,   175,   176,   177,
     178,   150,   154,    55,   151,   155,   314,   315,   245,    56,
     247,   181,   249,   250,   251,   295,   253,   254,   255,   256,
     302,   303,    57,   130,   327,   328,   131,   132,   286,    59,
     134,   313,   316,   295,   189,    58,   190,   319,   320,   228,
     229,   322,   323,   191,    41,   192,    60,    42,   329,    43,
      44,    45,    46,    61,   331,   332,   173,   174,   175,   176,
     177,   178,    84,   193,   195,   194,   196,    85,    86,    87,
      88,   130,   264,    62,   131,   132,   133,    63,   134,   283,
     284,   289,   290,   291,   130,    68,    64,   131,   132,   226,
     130,   134,    69,   131,   132,   230,   130,   134,    70,   131,
     132,   285,    71,   134,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    91,    92,    95,    96,
      97,    98,   100,   101,   109,   102,   105,   106,    65,   137,
     110,   111,   112,   138,   121,   128,   139,   148,   140,   149,
     170,   152,   153,   156,   166,   158,   160,   186,   188,   162,
     197,   164,   199,   124,   165,   198,   183,   206,   207,   215,
     210,   211,   212,   213,   216,   217,   219,   222,   236,   223,
     243,   262,   270,   271,   257,   272,   273,   274,   237,   238,
     268,   239,   234,   240,   275,   241,   276,   281,   288,   277,
     244,   246,   300,   295,   235,   248,   252,   263,   258,   259,
     266,   306,     0,   267,   187,   280,   282,   221,   125,   214,
       0,   301,     0,   279,   299,   287,     0,   298,     0,     0,
       0,     0,     0,     0,     0,     0,   304,     0,     0,     0,
       0,   305,     0,     0,   309,   317,   318,   321,   330
};

static const yytype_int16 yycheck[] =
{
      84,   128,   135,   183,   232,    89,    90,    19,    59,   126,
     127,   138,     6,    29,     8,    23,    24,    25,    26,   317,
     318,     6,    32,     8,    40,    41,    42,    43,    44,    45,
      18,   115,   330,   117,    46,   119,    18,    18,   122,    19,
      56,    29,    10,    11,    52,    18,     7,    29,    29,    29,
     278,    19,    18,   180,     3,   182,    29,    51,    30,    18,
       0,    29,    59,    29,     4,     5,   150,   151,   185,     9,
      29,    11,    12,    13,    14,    15,    16,   257,    53,    54,
      20,    21,    22,     3,    10,    11,    19,    27,    28,    18,
     223,    19,   219,    19,    19,    59,    29,    37,    62,    59,
      29,    29,    62,    29,    29,    40,    41,    42,    43,    44,
      45,    59,    59,     3,    62,    62,    10,    11,   202,     3,
     204,    56,   206,   207,   208,    19,   210,   211,   212,   213,
     293,   294,     3,    54,    10,    11,    57,    58,   265,    38,
      61,   304,   305,    19,    53,     3,    55,   310,   311,    53,
      54,   314,   315,    53,    59,    55,    59,    62,   321,    64,
      65,    66,    67,    59,   327,   328,    40,    41,    42,    43,
      44,    45,    59,    53,    53,    55,    55,    64,    65,    66,
      67,    54,    56,     8,    57,    58,    59,    59,    61,    53,
      54,   275,   276,   277,    54,    17,    59,    57,    58,    59,
      54,    61,    17,    57,    58,    59,    54,    61,    17,    57,
      58,    59,    17,    61,    32,     3,     3,    59,    59,    35,
      39,    17,    36,    59,     3,     3,    59,    59,    59,    59,
      31,    33,    59,    61,    17,    59,    59,    36,    19,     3,
      17,    17,    17,    40,    18,    17,    30,    17,    19,    59,
      19,    59,    59,    59,    47,    59,    59,     6,    18,    59,
      17,    59,    17,    19,    59,    59,    34,    18,    18,    59,
      18,    18,    18,    18,     3,    48,    19,     3,     3,    59,
      18,    18,     3,     3,    36,    18,     3,    18,    54,    54,
      29,    54,    59,    54,    18,    57,    18,     3,     3,    18,
      59,    59,    29,    19,   187,    59,    59,   221,    50,    49,
      54,    19,    -1,    54,   140,    50,   261,   170,    46,   165,
      -1,   292,    -1,    59,    54,    59,    -1,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    59,    -1,    -1,    -1,
      -1,    59,    -1,    -1,    59,    59,    59,    59,    59
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    69,     0,     4,     5,     9,    11,    12,    13,    14,
      15,    16,    20,    21,    22,    27,    28,    37,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    88,    93,    94,    95,   108,     6,     8,    51,     6,
       8,    59,    62,    64,    65,    66,    67,    97,    59,     7,
       3,    30,    32,    59,     3,     3,     3,     3,     3,    38,
      59,    59,     8,    59,    59,    19,    29,    98,    17,    17,
      17,    17,    32,     3,     3,    59,    59,    35,    39,    17,
      36,    59,     3,     3,    59,    64,    65,    66,    67,    59,
      62,    59,    59,    59,    62,    59,    59,    31,    33,   100,
      59,    61,    59,    84,    87,    59,    36,    29,    98,    17,
      17,    17,    17,    98,    98,    18,    29,    18,    29,    18,
      29,    18,    18,    29,    19,    46,    96,    99,    17,    90,
      54,    57,    58,    59,    61,    92,   106,     3,    40,    30,
      19,    83,    23,    24,    25,    26,    52,    86,    17,    59,
      59,    62,    59,    59,    59,    62,    59,    98,    59,    98,
      59,    98,    59,    98,    59,    59,    47,   100,   100,    92,
      19,    89,    29,    40,    41,    42,    43,    44,    45,    56,
     107,    56,   107,    34,   105,    92,     6,    84,    18,    53,
      55,    53,    55,    53,    55,    53,    55,    17,    59,    17,
      98,    98,    18,    29,    18,    29,    18,    18,    18,    29,
      18,    18,    18,    18,    99,    59,     3,    48,   101,    19,
      91,    90,     3,    59,    53,    54,    59,    92,    53,    54,
      59,    92,   106,   100,    59,    83,     3,    54,    54,    54,
      54,    57,    85,    18,    59,    98,    59,    98,    59,    98,
      98,    98,    59,    98,    98,    98,    98,    36,    50,    49,
     103,    92,    18,    89,    56,   107,    54,    54,    29,   105,
       3,     3,    18,     3,    18,    18,    18,    18,   106,    59,
      50,     3,    91,    53,    54,    59,    92,    59,     3,    98,
      98,    98,   105,    10,    11,    19,    29,   102,    59,    54,
      29,    96,   102,   102,    59,    59,    19,    29,   104,    59,
      10,    11,    29,   102,    10,    11,   102,    59,    59,   102,
     102,    59,   102,   102,    29,   104,   104,    10,    11,   102,
      59,   102,   102,   104
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    68,    69,    69,    70,    70,    70,    70,    70,    70,
      70,    70,    70,    70,    70,    70,    70,    70,    70,    70,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    80,    81,    82,    83,    83,    84,    84,    84,    85,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    87,    88,    89,    89,    90,    91,    91,    92,
      92,    92,    92,    93,    94,    95,    95,    96,    96,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    98,    98,    98,    98,    98,    98,    98,    98,
      98,    98,    98,    98,    99,    99,   100,   100,   101,   101,
     101,   101,   101,   101,   101,   102,   102,   102,   102,   102,
     102,   102,   103,   103,   103,   104,   104,   104,   105,   105,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   106,   106,   107,   107,   107,   107,   107,   107,   108
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     2,     2,     4,     3,     3,
       9,    10,     4,     8,     0,     3,     5,     2,     2,     1,
       1,     3,     2,     1,     3,     2,     1,     3,     2,     1,
       3,     2,     1,     7,     0,     3,     4,     0,     3,     1,
       1,     1,     1,     5,     8,     9,     7,     6,     7,     1,
       2,     4,     4,     5,     7,     5,     7,     5,     7,     4,
       5,     7,     0,     3,     5,     5,     6,     8,     6,     8,
       6,     6,     6,     8,     0,     3,     0,     3,     0,     4,
       5,     5,     6,     7,     7,     0,     3,     4,     4,     5,
       6,     6,     0,     4,     6,     0,     3,     5,     0,     3,
       3,     3,     3,     3,     5,     5,     7,     3,     4,     5,
       6,     3,     4,     1,     1,     1,     1,     1,     1,     8
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (scanner, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, void *scanner)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (scanner);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, void *scanner)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep, scanner);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule, void *scanner)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              , scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, scanner); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, void *scanner)
{
  YYUSE (yyvaluep);
  YYUSE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void *scanner)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs;

    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex (&yylval, scanner);
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 21:
#line 178 "yacc_sql.y"
                   {
        CONTEXT->ssql->flag=SCF_EXIT;//"exit";
    }
#line 1669 "yacc_sql.tab.c"
    break;

  case 22:
#line 183 "yacc_sql.y"
                   {
        CONTEXT->ssql->flag=SCF_HELP;//"help";
    }
#line 1677 "yacc_sql.tab.c"
    break;

  case 23:
#line 188 "yacc_sql.y"
                   {
      CONTEXT->ssql->flag = SCF_SYNC;
    }
#line 1685 "yacc_sql.tab.c"
    break;

  case 24:
#line 194 "yacc_sql.y"
                        {
      CONTEXT->ssql->flag = SCF_BEGIN;
    }
#line 1693 "yacc_sql.tab.c"
    break;

  case 25:
#line 200 "yacc_sql.y"
                         {
      CONTEXT->ssql->flag = SCF_COMMIT;
    }
#line 1701 "yacc_sql.tab.c"
    break;

  case 26:
#line 206 "yacc_sql.y"
                           {
      CONTEXT->ssql->flag = SCF_ROLLBACK;
    }
#line 1709 "yacc_sql.tab.c"
    break;

  case 27:
#line 212 "yacc_sql.y"
                            {
        CONTEXT->ssql->flag = SCF_DROP_TABLE;//"drop_table";
        drop_table_init(&CONTEXT->ssql->sstr.drop_table, (yyvsp[-1].string));
    }
#line 1718 "yacc_sql.tab.c"
    break;

  case 28:
#line 218 "yacc_sql.y"
                          {
      CONTEXT->ssql->flag = SCF_SHOW_TABLES;
    }
#line 1726 "yacc_sql.tab.c"
    break;

  case 29:
#line 224 "yacc_sql.y"
                      {
      CONTEXT->ssql->flag = SCF_DESC_TABLE;
      desc_table_init(&CONTEXT->ssql->sstr.desc_table, (yyvsp[-1].string));
    }
#line 1735 "yacc_sql.tab.c"
    break;

  case 30:
#line 232 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_index";
			create_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-6].string), (yyvsp[-4].string), (yyvsp[-2].string));
		}
#line 1744 "yacc_sql.tab.c"
    break;

  case 31:
#line 237 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_index";
			create_unique_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-6].string), (yyvsp[-4].string), (yyvsp[-2].string));
		}
#line 1753 "yacc_sql.tab.c"
    break;

  case 32:
#line 245 "yacc_sql.y"
                {
			CONTEXT->ssql->flag=SCF_DROP_INDEX;//"drop_index";
			drop_index_init(&CONTEXT->ssql->sstr.drop_index, (yyvsp[-1].string));
		}
#line 1762 "yacc_sql.tab.c"
    break;

  case 33:
#line 252 "yacc_sql.y"
                {
			CONTEXT->ssql->flag=SCF_CREATE_TABLE;//"create_table";
			// CONTEXT->ssql->sstr.create_table.attribute_count = CONTEXT->value_length;
			create_table_init_name(&CONTEXT->ssql->sstr.create_table, (yyvsp[-5].string));
			//临时变量清零	
			CONTEXT->value_length = 0;
		}
#line 1774 "yacc_sql.tab.c"
    break;

  case 35:
#line 262 "yacc_sql.y"
                                   {    }
#line 1780 "yacc_sql.tab.c"
    break;

  case 36:
#line 267 "yacc_sql.y"
                {
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, (yyvsp[-3].number), (yyvsp[-1].number));
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			CONTEXT->value_length++;
			if ((yyvsp[-3].number) == INTS_NULLABLE || (yyvsp[-3].number) == FLOATS_NULLABLE || (yyvsp[-3].number) == CHARS_NULLABLE || (yyvsp[-3].number) == DATES_NULLABLE) {
				attr_info_init(&attribute, "NULL", IF_NULL, 4);
				create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
				CONTEXT->value_length++;
			}
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name =(char*)malloc(sizeof(char));
			// strcpy(CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name, CONTEXT->id); 
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].type = $2;  
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].length = $4;
			
		}
#line 1801 "yacc_sql.tab.c"
    break;

  case 37:
#line 284 "yacc_sql.y"
                {
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, (yyvsp[0].number), 4);
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name=(char*)malloc(sizeof(char));
			// strcpy(CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name, CONTEXT->id); 
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].type=$2;  
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].length=4; // default attribute length
			CONTEXT->value_length++;
			if ((yyvsp[0].number) == INTS_NULLABLE || (yyvsp[0].number) == FLOATS_NULLABLE || (yyvsp[0].number) == CHARS_NULLABLE || (yyvsp[0].number) == DATES_NULLABLE) {
				attr_info_init(&attribute, "NULL", IF_NULL, 4);
				create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
				CONTEXT->value_length++;
			}
		}
#line 1821 "yacc_sql.tab.c"
    break;

  case 38:
#line 300 "yacc_sql.y"
                {
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, CHARS, 4096);
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			CONTEXT->value_length++;
		}
#line 1832 "yacc_sql.tab.c"
    break;

  case 39:
#line 308 "yacc_sql.y"
                       {(yyval.number) = (yyvsp[0].number);}
#line 1838 "yacc_sql.tab.c"
    break;

  case 40:
#line 311 "yacc_sql.y"
              { (yyval.number)=INTS; }
#line 1844 "yacc_sql.tab.c"
    break;

  case 41:
#line 312 "yacc_sql.y"
                           { (yyval.number)=INTS; }
#line 1850 "yacc_sql.tab.c"
    break;

  case 42:
#line 313 "yacc_sql.y"
                         { (yyval.number)=INTS_NULLABLE; }
#line 1856 "yacc_sql.tab.c"
    break;

  case 43:
#line 314 "yacc_sql.y"
               { (yyval.number)=CHARS; }
#line 1862 "yacc_sql.tab.c"
    break;

  case 44:
#line 315 "yacc_sql.y"
                              { (yyval.number)=CHARS; }
#line 1868 "yacc_sql.tab.c"
    break;

  case 45:
#line 316 "yacc_sql.y"
                            { (yyval.number)=CHARS_NULLABLE; }
#line 1874 "yacc_sql.tab.c"
    break;

  case 46:
#line 317 "yacc_sql.y"
              { (yyval.number)=FLOATS; }
#line 1880 "yacc_sql.tab.c"
    break;

  case 47:
#line 318 "yacc_sql.y"
                             { (yyval.number)=FLOATS; }
#line 1886 "yacc_sql.tab.c"
    break;

  case 48:
#line 319 "yacc_sql.y"
                           { (yyval.number)=FLOATS_NULLABLE; }
#line 1892 "yacc_sql.tab.c"
    break;

  case 49:
#line 320 "yacc_sql.y"
                 { (yyval.number)=DATES; }
#line 1898 "yacc_sql.tab.c"
    break;

  case 50:
#line 321 "yacc_sql.y"
                            { (yyval.number)=DATES; }
#line 1904 "yacc_sql.tab.c"
    break;

  case 51:
#line 322 "yacc_sql.y"
                          { (yyval.number)=DATES_NULLABLE; }
#line 1910 "yacc_sql.tab.c"
    break;

  case 52:
#line 326 "yacc_sql.y"
        {
		char *temp=(yyvsp[0].string); 
		snprintf(CONTEXT->id, sizeof(CONTEXT->id), "%s", temp);
	}
#line 1919 "yacc_sql.tab.c"
    break;

  case 53:
#line 335 "yacc_sql.y"
                {
			// CONTEXT->values[CONTEXT->value_length++] = *$6;

			CONTEXT->ssql->flag=SCF_INSERT;//"insert";
			// CONTEXT->ssql->sstr.insertion.relation_name = $3;
			// CONTEXT->ssql->sstr.insertion.value_num = CONTEXT->value_length;
			// for(i = 0; i < CONTEXT->value_length; i++){
			// 	CONTEXT->ssql->sstr.insertion.values[i] = CONTEXT->values[i];
      // }
			inserts_init(&CONTEXT->ssql->sstr.insertion, (yyvsp[-4].string), CONTEXT->values, CONTEXT->value_length, CONTEXT->data_num);

      //临时变量清零
      CONTEXT->value_length=0;
	  CONTEXT->data_num=0;
    }
#line 1939 "yacc_sql.tab.c"
    break;

  case 55:
#line 353 "yacc_sql.y"
                                        { 
  		// CONTEXT->values[CONTEXT->value_length++] = *$2;
	  }
#line 1947 "yacc_sql.tab.c"
    break;

  case 56:
#line 358 "yacc_sql.y"
                                       {
		CONTEXT->data_num++;
	}
#line 1955 "yacc_sql.tab.c"
    break;

  case 58:
#line 365 "yacc_sql.y"
                              { 
  		// CONTEXT->values[CONTEXT->value_length++] = *$2;
	  }
#line 1963 "yacc_sql.tab.c"
    break;

  case 59:
#line 370 "yacc_sql.y"
          {	
  		value_init_integer(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].number));
		}
#line 1971 "yacc_sql.tab.c"
    break;

  case 60:
#line 373 "yacc_sql.y"
          {
  		value_init_float(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].floats));
		}
#line 1979 "yacc_sql.tab.c"
    break;

  case 61:
#line 376 "yacc_sql.y"
         {
			(yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
  		value_init_string(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].string));
		}
#line 1988 "yacc_sql.tab.c"
    break;

  case 62:
#line 380 "yacc_sql.y"
            {
		// $1 = substr($1,1,strlen($1)-2);
  		// value_init_string(&CONTEXT->values[CONTEXT->value_length++], "null");
		  value_init_null(&CONTEXT->values[CONTEXT->value_length++]);
	}
#line 1998 "yacc_sql.tab.c"
    break;

  case 63:
#line 388 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_DELETE;//"delete";
			deletes_init_relation(&CONTEXT->ssql->sstr.deletion, (yyvsp[-2].string));
			deletes_set_conditions(&CONTEXT->ssql->sstr.deletion, 
					CONTEXT->conditions, CONTEXT->condition_length);
			CONTEXT->condition_length = 0;	
    }
#line 2010 "yacc_sql.tab.c"
    break;

  case 64:
#line 398 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_UPDATE;//"update";
			Value *value = &CONTEXT->values[0];
			updates_init(&CONTEXT->ssql->sstr.update, (yyvsp[-6].string), (yyvsp[-4].string), value, 
					CONTEXT->conditions, CONTEXT->condition_length);
			CONTEXT->condition_length = 0;
		}
#line 2022 "yacc_sql.tab.c"
    break;

  case 65:
#line 408 "yacc_sql.y"
                {
			// CONTEXT->ssql->sstr.selection.relations[CONTEXT->from_length++]=$4;
			selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-5].string));

			selects_append_conditions(&CONTEXT->ssql->sstr.selection, CONTEXT->conditions, CONTEXT->condition_length);

			CONTEXT->ssql->flag=SCF_SELECT;//"select";
			// CONTEXT->ssql->sstr.selection.attr_num = CONTEXT->select_length;

			//临时变量清零
			CONTEXT->condition_length=0;
			CONTEXT->from_length=0;
			CONTEXT->select_length=0;
			CONTEXT->value_length = 0;
	}
#line 2042 "yacc_sql.tab.c"
    break;

  case 66:
#line 424 "yacc_sql.y"
        {
		selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-3].string));
		selects_append_conditions(&CONTEXT->ssql->sstr.selection, CONTEXT->conditions, CONTEXT->condition_length);
		CONTEXT->ssql->flag=SCF_SELECT;//"select";
		// CONTEXT->ssql->sstr.selection.attr_num = CONTEXT->select_length;

		//临时变量清零
		CONTEXT->condition_length=0;
		CONTEXT->from_length=0;
		CONTEXT->select_length=0;
		CONTEXT->value_length = 0;
	}
#line 2059 "yacc_sql.tab.c"
    break;

  case 67:
#line 439 "yacc_sql.y"
                                                  {
		selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-3].string));
	}
#line 2067 "yacc_sql.tab.c"
    break;

  case 68:
#line 442 "yacc_sql.y"
                                                              {
		selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-4].string));
	}
#line 2075 "yacc_sql.tab.c"
    break;

  case 69:
#line 447 "yacc_sql.y"
         {  
			RelAttr attr;
			relation_attr_init(&attr, NULL, "*");
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 2085 "yacc_sql.tab.c"
    break;

  case 70:
#line 452 "yacc_sql.y"
                  {
			RelAttr attr;
			relation_attr_init(&attr, NULL, (yyvsp[-1].string));
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 2095 "yacc_sql.tab.c"
    break;

  case 71:
#line 457 "yacc_sql.y"
                              {
			RelAttr attr;
			relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string));
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 2105 "yacc_sql.tab.c"
    break;

  case 72:
#line 462 "yacc_sql.y"
                                {
			RelAttr attr;
			relation_attr_init(&attr, (yyvsp[-3].string), "*");
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 2115 "yacc_sql.tab.c"
    break;

  case 73:
#line 467 "yacc_sql.y"
                                        {
			RelAttr attr;
			char* s=malloc(sizeof(char)*(3+strlen((yyvsp[-2].string))+3));
			strcpy(s, "MAX(");
			// strcat(s, "(");
			strcat(s, (yyvsp[-2].string));
			strcat(s, ")");
			relation_attr_init(&attr, NULL, s);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2130 "yacc_sql.tab.c"
    break;

  case 74:
#line 477 "yacc_sql.y"
                                               {
			RelAttr attr;
			char* s=malloc(sizeof(char)*(3+strlen((yyvsp[-2].string))+3));
			strcpy(s, "MAX(");
			// strcat(s, "(");
			strcat(s, (yyvsp[-2].string));
			strcat(s, ")");
			relation_attr_init(&attr, (yyvsp[-4].string), s);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2145 "yacc_sql.tab.c"
    break;

  case 75:
#line 487 "yacc_sql.y"
                                        {
			RelAttr attr;
			char* s=malloc(sizeof(char)*(3+strlen((yyvsp[-2].string))+3));
			strcpy(s, "MIN(");
			// strcat(s, "(");
			strcat(s, (yyvsp[-2].string));
			strcat(s, ")");
			relation_attr_init(&attr, NULL, s);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2160 "yacc_sql.tab.c"
    break;

  case 76:
#line 497 "yacc_sql.y"
                                               {
			RelAttr attr;
			char* s=malloc(sizeof(char)*(3+strlen((yyvsp[-2].string))+3));
			strcpy(s, "MIN(");
			// strcat(s, "(");
			strcat(s, (yyvsp[-2].string));
			strcat(s, ")");
			relation_attr_init(&attr, (yyvsp[-4].string), s);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2175 "yacc_sql.tab.c"
    break;

  case 77:
#line 507 "yacc_sql.y"
                                          {
			RelAttr attr;
			char* s=malloc(sizeof(char)*(5+strlen((yyvsp[-2].string))+3));
			strcpy(s, "COUNT(");
			// strcat(s, "(");
			strcat(s, (yyvsp[-2].string));
			strcat(s, ")");
			relation_attr_init(&attr, NULL, s);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2190 "yacc_sql.tab.c"
    break;

  case 78:
#line 517 "yacc_sql.y"
                                                 {
			RelAttr attr;
			char* s=malloc(sizeof(char)*(5+strlen((yyvsp[-2].string))+3));
			strcpy(s, "COUNT(");
			// strcat(s, "(");
			strcat(s, (yyvsp[-2].string));
			strcat(s, ")");
			relation_attr_init(&attr, (yyvsp[-4].string), s);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2205 "yacc_sql.tab.c"
    break;

  case 79:
#line 527 "yacc_sql.y"
                                   {
			RelAttr attr;
			// char* s=malloc(sizeof(char)*(strlen($1)+4));
			// strcpy(s, $1);
			// strcat(s, "(*)");
			// strcat(s, $3);
			relation_attr_init(&attr, NULL, "COUNT(*)");
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2219 "yacc_sql.tab.c"
    break;

  case 80:
#line 536 "yacc_sql.y"
                                        {
			RelAttr attr;
			char* s=malloc(sizeof(char)*(3+strlen((yyvsp[-2].string))+3));
			strcpy(s, "AVG(");
			strcat(s, (yyvsp[-2].string));
			strcat(s, ")");
			relation_attr_init(&attr, NULL, s);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2233 "yacc_sql.tab.c"
    break;

  case 81:
#line 545 "yacc_sql.y"
                                               {
			RelAttr attr;
			char* s=malloc(sizeof(char)*(3+strlen((yyvsp[-2].string))+3));
			strcpy(s, "AVG(");
			strcat(s, (yyvsp[-2].string));
			strcat(s, ")");
			relation_attr_init(&attr, (yyvsp[-4].string), s);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2247 "yacc_sql.tab.c"
    break;

  case 83:
#line 559 "yacc_sql.y"
                         {
			RelAttr attr;
			relation_attr_init(&attr, NULL, (yyvsp[-1].string));
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
     	  // CONTEXT->ssql->sstr.selection.attributes[CONTEXT->select_length].relation_name = NULL;
        // CONTEXT->ssql->sstr.selection.attributes[CONTEXT->select_length++].attribute_name=$2;
      }
#line 2259 "yacc_sql.tab.c"
    break;

  case 84:
#line 566 "yacc_sql.y"
                                {
			RelAttr attr;
			relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string));
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
        // CONTEXT->ssql->sstr.selection.attributes[CONTEXT->select_length].attribute_name=$4;
        // CONTEXT->ssql->sstr.selection.attributes[CONTEXT->select_length++].relation_name=$2;
  	  }
#line 2271 "yacc_sql.tab.c"
    break;

  case 85:
#line 573 "yacc_sql.y"
                                      {
			RelAttr attr;
			relation_attr_init(&attr, (yyvsp[-3].string), "*");
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
        // CONTEXT->ssql->sstr.selection.attributes[CONTEXT->select_length].attribute_name=$4;
        // CONTEXT->ssql->sstr.selection.attributes[CONTEXT->select_length++].relation_name=$2;
  	  }
#line 2283 "yacc_sql.tab.c"
    break;

  case 86:
#line 580 "yacc_sql.y"
                                               {
			RelAttr attr;
			char* s=malloc(sizeof(char)*(3+strlen((yyvsp[-2].string))+3));
			strcpy(s, "MAX(");
			// strcat(s, "(");
			strcat(s, (yyvsp[-2].string));
			strcat(s, ")");
			relation_attr_init(&attr, NULL, s);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2298 "yacc_sql.tab.c"
    break;

  case 87:
#line 590 "yacc_sql.y"
                                                      {
			RelAttr attr;
			char* s=malloc(sizeof(char)*(3+strlen((yyvsp[-2].string))+3));
			strcpy(s, "MAX(");
			// strcat(s, "(");
			strcat(s, (yyvsp[-2].string));
			strcat(s, ")");
			relation_attr_init(&attr, (yyvsp[-4].string), s);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2313 "yacc_sql.tab.c"
    break;

  case 88:
#line 600 "yacc_sql.y"
                                               {
			RelAttr attr;
			char* s=malloc(sizeof(char)*(3+strlen((yyvsp[-2].string))+3));
			strcpy(s, "MIN(");
			// strcat(s, "(");
			strcat(s, (yyvsp[-2].string));
			strcat(s, ")");
			relation_attr_init(&attr, NULL, s);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2328 "yacc_sql.tab.c"
    break;

  case 89:
#line 610 "yacc_sql.y"
                                                      {
			RelAttr attr;
			char* s=malloc(sizeof(char)*(3+strlen((yyvsp[-2].string))+3));
			strcpy(s, "MIN(");
			// strcat(s, "(");
			strcat(s, (yyvsp[-2].string));
			strcat(s, ")");
			relation_attr_init(&attr, (yyvsp[-4].string), s);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2343 "yacc_sql.tab.c"
    break;

  case 90:
#line 620 "yacc_sql.y"
                                                 {
			RelAttr attr;
			char* s=malloc(sizeof(char)*(5+strlen((yyvsp[-2].string))+3));
			strcpy(s, "COUNT(");
			// strcat(s, "(");
			strcat(s, (yyvsp[-2].string));
			strcat(s, ")");
			relation_attr_init(&attr, NULL, s);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2358 "yacc_sql.tab.c"
    break;

  case 91:
#line 630 "yacc_sql.y"
                                                   {
			RelAttr attr;
			relation_attr_init(&attr, NULL, "COUNT(*)");
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2368 "yacc_sql.tab.c"
    break;

  case 92:
#line 635 "yacc_sql.y"
                                               {
			RelAttr attr;
			char* s=malloc(sizeof(char)*(3+strlen((yyvsp[-2].string))+3));
			strcpy(s, "AVG(");
			// strcat(s, "(");
			strcat(s, (yyvsp[-2].string));
			strcat(s, ")");
			relation_attr_init(&attr, NULL, s);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2383 "yacc_sql.tab.c"
    break;

  case 93:
#line 645 "yacc_sql.y"
                                                      {
			RelAttr attr;
			char* s=malloc(sizeof(char)*(3+strlen((yyvsp[-2].string))+3));
			strcpy(s, "AVG(");
			// strcat(s, "(");
			strcat(s, (yyvsp[-2].string));
			strcat(s, ")");
			relation_attr_init(&attr, (yyvsp[-4].string), s);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2398 "yacc_sql.tab.c"
    break;

  case 95:
#line 736 "yacc_sql.y"
                        {	
				selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-1].string));
		  }
#line 2406 "yacc_sql.tab.c"
    break;

  case 97:
#line 742 "yacc_sql.y"
                                     {	
				// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
			}
#line 2414 "yacc_sql.tab.c"
    break;

  case 99:
#line 749 "yacc_sql.y"
                                {	
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-1].string));
			Condition condition;
			condition_init(&condition, ORDER_BY_ASC, 1, &left_attr, NULL, 1, &left_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2426 "yacc_sql.tab.c"
    break;

  case 100:
#line 756 "yacc_sql.y"
                                        {	
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-2].string));
			Condition condition;
			condition_init(&condition, ORDER_BY_ASC, 1, &left_attr, NULL, 1, &left_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2438 "yacc_sql.tab.c"
    break;

  case 101:
#line 763 "yacc_sql.y"
                                         {	
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-2].string));
			Condition condition;
			condition_init(&condition, ORDER_BY_DESC, 1, &left_attr, NULL, 1, &left_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2450 "yacc_sql.tab.c"
    break;

  case 102:
#line 770 "yacc_sql.y"
                                           {	
			RelAttr left_attr;
			relation_attr_init(&left_attr, (yyvsp[-3].string), (yyvsp[-1].string));
			Condition condition;
			condition_init(&condition, ORDER_BY_ASC, 1, &left_attr, NULL, 1, &left_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
			}
#line 2462 "yacc_sql.tab.c"
    break;

  case 103:
#line 777 "yacc_sql.y"
                                               {	
			RelAttr left_attr;
			relation_attr_init(&left_attr, (yyvsp[-4].string), (yyvsp[-2].string));
			Condition condition;
			condition_init(&condition, ORDER_BY_ASC, 1, &left_attr, NULL, 1, &left_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
			}
#line 2474 "yacc_sql.tab.c"
    break;

  case 104:
#line 784 "yacc_sql.y"
                                                {	
			RelAttr left_attr;
			relation_attr_init(&left_attr, (yyvsp[-4].string), (yyvsp[-2].string));
			Condition condition;
			condition_init(&condition, ORDER_BY_DESC, 1, &left_attr, NULL, 1, &left_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
			}
#line 2486 "yacc_sql.tab.c"
    break;

  case 106:
#line 795 "yacc_sql.y"
                           {
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-1].string));
			Condition condition;
			condition_init(&condition, ORDER_BY_ASC, 1, &left_attr, NULL, 1, &left_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2498 "yacc_sql.tab.c"
    break;

  case 107:
#line 802 "yacc_sql.y"
                                   {
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-2].string));
			Condition condition;
			condition_init(&condition, ORDER_BY_ASC, 1, &left_attr, NULL, 1, &left_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2510 "yacc_sql.tab.c"
    break;

  case 108:
#line 809 "yacc_sql.y"
                                    {
		RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-2].string));
			Condition condition;
			condition_init(&condition, ORDER_BY_DESC, 1, &left_attr, NULL, 1, &left_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2522 "yacc_sql.tab.c"
    break;

  case 109:
#line 816 "yacc_sql.y"
                                      {
		RelAttr left_attr;
			relation_attr_init(&left_attr, (yyvsp[-3].string), (yyvsp[-1].string));
			Condition condition;
			condition_init(&condition, ORDER_BY_ASC, 1, &left_attr, NULL, 1, &left_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2534 "yacc_sql.tab.c"
    break;

  case 110:
#line 823 "yacc_sql.y"
                                          {
		RelAttr left_attr;
			relation_attr_init(&left_attr, (yyvsp[-4].string), (yyvsp[-2].string));
			Condition condition;
			condition_init(&condition, ORDER_BY_ASC, 1, &left_attr, NULL, 1, &left_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2546 "yacc_sql.tab.c"
    break;

  case 111:
#line 830 "yacc_sql.y"
                                           {
		RelAttr left_attr;
			relation_attr_init(&left_attr, (yyvsp[-4].string), (yyvsp[-2].string));
			Condition condition;
			condition_init(&condition, ORDER_BY_DESC, 1, &left_attr, NULL, 1, &left_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2558 "yacc_sql.tab.c"
    break;

  case 113:
#line 841 "yacc_sql.y"
                                {	
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-1].string));
			Condition condition;
			condition_init(&condition, GROUP_BY, 1, &left_attr, NULL, 1, &left_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2570 "yacc_sql.tab.c"
    break;

  case 114:
#line 848 "yacc_sql.y"
                                           {	
			RelAttr left_attr;
			relation_attr_init(&left_attr, (yyvsp[-3].string), (yyvsp[-1].string));
			Condition condition;
			condition_init(&condition, GROUP_BY, 1, &left_attr, NULL, 1, &left_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
			}
#line 2582 "yacc_sql.tab.c"
    break;

  case 116:
#line 859 "yacc_sql.y"
                           {
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-1].string));
			Condition condition;
			condition_init(&condition, GROUP_BY, 1, &left_attr, NULL, 1, &left_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2594 "yacc_sql.tab.c"
    break;

  case 117:
#line 866 "yacc_sql.y"
                                      {
		RelAttr left_attr;
			relation_attr_init(&left_attr, (yyvsp[-3].string), (yyvsp[-1].string));
			Condition condition;
			condition_init(&condition, GROUP_BY, 1, &left_attr, NULL, 1, &left_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2606 "yacc_sql.tab.c"
    break;

  case 119:
#line 878 "yacc_sql.y"
                                   {
				// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
			}
#line 2614 "yacc_sql.tab.c"
    break;

  case 120:
#line 884 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-2].string));

			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 0, NULL, right_value);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
			// $$ = ( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 1;
			// $$->left_attr.relation_name = NULL;
			// $$->left_attr.attribute_name= $1;
			// $$->comp = CONTEXT->comp;
			// $$->right_is_attr = 0;
			// $$->right_attr.relation_name = NULL;
			// $$->right_attr.attribute_name = NULL;
			// $$->right_value = *$3;

		}
#line 2639 "yacc_sql.tab.c"
    break;

  case 121:
#line 905 "yacc_sql.y"
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 2];
			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 0, NULL, left_value, 0, NULL, right_value);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
			// $$ = ( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 0;
			// $$->left_attr.relation_name=NULL;
			// $$->left_attr.attribute_name=NULL;
			// $$->left_value = *$1;
			// $$->comp = CONTEXT->comp;
			// $$->right_is_attr = 0;
			// $$->right_attr.relation_name = NULL;
			// $$->right_attr.attribute_name = NULL;
			// $$->right_value = *$3;

		}
#line 2663 "yacc_sql.tab.c"
    break;

  case 122:
#line 925 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-2].string));
			RelAttr right_attr;
			relation_attr_init(&right_attr, NULL, (yyvsp[0].string));

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 1, &right_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
			// $$=( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 1;
			// $$->left_attr.relation_name=NULL;
			// $$->left_attr.attribute_name=$1;
			// $$->comp = CONTEXT->comp;
			// $$->right_is_attr = 1;
			// $$->right_attr.relation_name=NULL;
			// $$->right_attr.attribute_name=$3;

		}
#line 2687 "yacc_sql.tab.c"
    break;

  case 123:
#line 945 "yacc_sql.y"
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];
			RelAttr right_attr;
			relation_attr_init(&right_attr, NULL, (yyvsp[0].string));

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 0, NULL, left_value, 1, &right_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;

			// $$=( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 0;
			// $$->left_attr.relation_name=NULL;
			// $$->left_attr.attribute_name=NULL;
			// $$->left_value = *$1;
			// $$->comp=CONTEXT->comp;
			
			// $$->right_is_attr = 1;
			// $$->right_attr.relation_name=NULL;
			// $$->right_attr.attribute_name=$3;
		
		}
#line 2713 "yacc_sql.tab.c"
    break;

  case 124:
#line 967 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, (yyvsp[-4].string), (yyvsp[-2].string));
			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 0, NULL, right_value);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;

			// $$=( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 1;
			// $$->left_attr.relation_name=$1;
			// $$->left_attr.attribute_name=$3;
			// $$->comp=CONTEXT->comp;
			// $$->right_is_attr = 0;   //属性值
			// $$->right_attr.relation_name=NULL;
			// $$->right_attr.attribute_name=NULL;
			// $$->right_value =*$5;			
							
    }
#line 2738 "yacc_sql.tab.c"
    break;

  case 125:
#line 988 "yacc_sql.y"
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];

			RelAttr right_attr;
			relation_attr_init(&right_attr, (yyvsp[-2].string), (yyvsp[0].string));

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 0, NULL, left_value, 1, &right_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
			// $$=( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 0;//属性值
			// $$->left_attr.relation_name=NULL;
			// $$->left_attr.attribute_name=NULL;
			// $$->left_value = *$1;
			// $$->comp =CONTEXT->comp;
			// $$->right_is_attr = 1;//属性
			// $$->right_attr.relation_name = $3;
			// $$->right_attr.attribute_name = $5;
									
    }
#line 2763 "yacc_sql.tab.c"
    break;

  case 126:
#line 1009 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, (yyvsp[-6].string), (yyvsp[-4].string));
			RelAttr right_attr;
			relation_attr_init(&right_attr, (yyvsp[-2].string), (yyvsp[0].string));

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 1, &right_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
			// $$=( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 1;		//属性
			// $$->left_attr.relation_name=$1;
			// $$->left_attr.attribute_name=$3;
			// $$->comp =CONTEXT->comp;
			// $$->right_is_attr = 1;		//属性
			// $$->right_attr.relation_name=$5;
			// $$->right_attr.attribute_name=$7;
    }
#line 2786 "yacc_sql.tab.c"
    break;

  case 127:
#line 1027 "yacc_sql.y"
                     {
		RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-2].string));
			value_init_null(&CONTEXT->values[CONTEXT->value_length++]);
			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];
			Condition condition;
			condition_init(&condition, IS, 1, &left_attr, NULL, 0, NULL, right_value);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2800 "yacc_sql.tab.c"
    break;

  case 128:
#line 1036 "yacc_sql.y"
                             {
		RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-3].string));
			value_init_null(&CONTEXT->values[CONTEXT->value_length++]);
			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];
			Condition condition;
			condition_init(&condition, IS_NOT, 1, &left_attr, NULL, 0, NULL, right_value);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2814 "yacc_sql.tab.c"
    break;

  case 129:
#line 1045 "yacc_sql.y"
                                {
		RelAttr left_attr;
			relation_attr_init(&left_attr, (yyvsp[-4].string), (yyvsp[-2].string));
			value_init_null(&CONTEXT->values[CONTEXT->value_length++]);
			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];
			Condition condition;
			condition_init(&condition, IS, 1, &left_attr, NULL, 0, NULL, right_value);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2828 "yacc_sql.tab.c"
    break;

  case 130:
#line 1054 "yacc_sql.y"
                                   {
		RelAttr left_attr;
			relation_attr_init(&left_attr, (yyvsp[-5].string), (yyvsp[-3].string));
			value_init_null(&CONTEXT->values[CONTEXT->value_length++]);
			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];
			Condition condition;
			condition_init(&condition, IS_NOT, 1, &left_attr, NULL, 0, NULL, right_value);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
	}
#line 2842 "yacc_sql.tab.c"
    break;

  case 131:
#line 1064 "yacc_sql.y"
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];
			value_init_null(&CONTEXT->values[CONTEXT->value_length++]);
			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];
			Condition condition;
			condition_init(&condition, IS, 0, NULL, left_value, 0, NULL, right_value);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
		
		}
#line 2856 "yacc_sql.tab.c"
    break;

  case 132:
#line 1074 "yacc_sql.y"
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];
			value_init_null(&CONTEXT->values[CONTEXT->value_length++]);
			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];
			Condition condition;
			condition_init(&condition, IS_NOT, 0, NULL, left_value, 0, NULL, right_value);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
		
		}
#line 2870 "yacc_sql.tab.c"
    break;

  case 133:
#line 1086 "yacc_sql.y"
             { CONTEXT->comp = EQUAL_TO; }
#line 2876 "yacc_sql.tab.c"
    break;

  case 134:
#line 1087 "yacc_sql.y"
         { CONTEXT->comp = LESS_THAN; }
#line 2882 "yacc_sql.tab.c"
    break;

  case 135:
#line 1088 "yacc_sql.y"
         { CONTEXT->comp = GREAT_THAN; }
#line 2888 "yacc_sql.tab.c"
    break;

  case 136:
#line 1089 "yacc_sql.y"
         { CONTEXT->comp = LESS_EQUAL; }
#line 2894 "yacc_sql.tab.c"
    break;

  case 137:
#line 1090 "yacc_sql.y"
         { CONTEXT->comp = GREAT_EQUAL; }
#line 2900 "yacc_sql.tab.c"
    break;

  case 138:
#line 1091 "yacc_sql.y"
         { CONTEXT->comp = NOT_EQUAL; }
#line 2906 "yacc_sql.tab.c"
    break;

  case 139:
#line 1096 "yacc_sql.y"
                {
		  CONTEXT->ssql->flag = SCF_LOAD_DATA;
			load_data_init(&CONTEXT->ssql->sstr.load_data, (yyvsp[-1].string), (yyvsp[-4].string));
		}
#line 2915 "yacc_sql.tab.c"
    break;


#line 2919 "yacc_sql.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (scanner, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (scanner, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, scanner);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (scanner, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp, scanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1101 "yacc_sql.y"

//_____________________________________________________________________
extern void scan_string(const char *str, yyscan_t scanner);

int sql_parse(const char *s, Query *sqls){
	ParserContext context;
	memset(&context, 0, sizeof(context));

	yyscan_t scanner;
	yylex_init_extra(&context, &scanner);
	context.ssql = sqls;
	scan_string(s, scanner);
	int result = yyparse(scanner);
	yylex_destroy(scanner);
	return result;
}
