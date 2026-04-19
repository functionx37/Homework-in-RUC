/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

#include <stdio.h>
#include <stdlib.h>

extern int yylex(void);
extern FILE* yyin;

FILE* fout;

void yyerror(const char* s);

#define P(s) fprintf(fout, "%s\n", s)

#line 85 "parser.tab.c"

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

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_CONSTTK = 3,                    /* CONSTTK  */
  YYSYMBOL_INTTK = 4,                      /* INTTK  */
  YYSYMBOL_VOIDTK = 5,                     /* VOIDTK  */
  YYSYMBOL_IFTK = 6,                       /* IFTK  */
  YYSYMBOL_ELSETK = 7,                     /* ELSETK  */
  YYSYMBOL_WHILETK = 8,                    /* WHILETK  */
  YYSYMBOL_BREAKTK = 9,                    /* BREAKTK  */
  YYSYMBOL_CONTINUETK = 10,                /* CONTINUETK  */
  YYSYMBOL_RETURNTK = 11,                  /* RETURNTK  */
  YYSYMBOL_PRINTFTK = 12,                  /* PRINTFTK  */
  YYSYMBOL_IDENFR = 13,                    /* IDENFR  */
  YYSYMBOL_INTCON = 14,                    /* INTCON  */
  YYSYMBOL_STRCON = 15,                    /* STRCON  */
  YYSYMBOL_LEQ = 16,                       /* LEQ  */
  YYSYMBOL_GEQ = 17,                       /* GEQ  */
  YYSYMBOL_EQL = 18,                       /* EQL  */
  YYSYMBOL_NEQ = 19,                       /* NEQ  */
  YYSYMBOL_AND = 20,                       /* AND  */
  YYSYMBOL_OR = 21,                        /* OR  */
  YYSYMBOL_PLUS = 22,                      /* PLUS  */
  YYSYMBOL_MINU = 23,                      /* MINU  */
  YYSYMBOL_MULT = 24,                      /* MULT  */
  YYSYMBOL_DIV = 25,                       /* DIV  */
  YYSYMBOL_MOD = 26,                       /* MOD  */
  YYSYMBOL_NOT = 27,                       /* NOT  */
  YYSYMBOL_LSS = 28,                       /* LSS  */
  YYSYMBOL_GRE = 29,                       /* GRE  */
  YYSYMBOL_ASSIGN = 30,                    /* ASSIGN  */
  YYSYMBOL_SEMICN = 31,                    /* SEMICN  */
  YYSYMBOL_COMMA = 32,                     /* COMMA  */
  YYSYMBOL_LPARENT = 33,                   /* LPARENT  */
  YYSYMBOL_RPARENT = 34,                   /* RPARENT  */
  YYSYMBOL_LBRACK = 35,                    /* LBRACK  */
  YYSYMBOL_RBRACK = 36,                    /* RBRACK  */
  YYSYMBOL_LBRACE = 37,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 38,                    /* RBRACE  */
  YYSYMBOL_LOWER_THAN_ELSE = 39,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_YYACCEPT = 40,                  /* $accept  */
  YYSYMBOL_CompUnit = 41,                  /* CompUnit  */
  YYSYMBOL_CompUnitItem = 42,              /* CompUnitItem  */
  YYSYMBOL_43_1 = 43,                      /* $@1  */
  YYSYMBOL_IntIdentRest = 44,              /* IntIdentRest  */
  YYSYMBOL_45_2 = 45,                      /* $@2  */
  YYSYMBOL_46_3 = 46,                      /* $@3  */
  YYSYMBOL_VarDefSuffix = 47,              /* VarDefSuffix  */
  YYSYMBOL_VarDefListTail = 48,            /* VarDefListTail  */
  YYSYMBOL_ConstDecl = 49,                 /* ConstDecl  */
  YYSYMBOL_ConstDefList = 50,              /* ConstDefList  */
  YYSYMBOL_ConstDef = 51,                  /* ConstDef  */
  YYSYMBOL_ConstDims = 52,                 /* ConstDims  */
  YYSYMBOL_ConstInitVal = 53,              /* ConstInitVal  */
  YYSYMBOL_ConstInitValList = 54,          /* ConstInitValList  */
  YYSYMBOL_VarDecl = 55,                   /* VarDecl  */
  YYSYMBOL_VarDefList = 56,                /* VarDefList  */
  YYSYMBOL_VarDef = 57,                    /* VarDef  */
  YYSYMBOL_VarDefDims = 58,                /* VarDefDims  */
  YYSYMBOL_InitVal = 59,                   /* InitVal  */
  YYSYMBOL_InitValList = 60,               /* InitValList  */
  YYSYMBOL_FuncFParamsOpt = 61,            /* FuncFParamsOpt  */
  YYSYMBOL_FuncFParamsList = 62,           /* FuncFParamsList  */
  YYSYMBOL_FuncFParam = 63,                /* FuncFParam  */
  YYSYMBOL_FuncFParamDims = 64,            /* FuncFParamDims  */
  YYSYMBOL_Block = 65,                     /* Block  */
  YYSYMBOL_BlockItems = 66,                /* BlockItems  */
  YYSYMBOL_BlockItem = 67,                 /* BlockItem  */
  YYSYMBOL_Stmt = 68,                      /* Stmt  */
  YYSYMBOL_FormatString = 69,              /* FormatString  */
  YYSYMBOL_PrintfArgsTail = 70,            /* PrintfArgsTail  */
  YYSYMBOL_Exp = 71,                       /* Exp  */
  YYSYMBOL_Cond = 72,                      /* Cond  */
  YYSYMBOL_ConstExp = 73,                  /* ConstExp  */
  YYSYMBOL_LOrExp = 74,                    /* LOrExp  */
  YYSYMBOL_LAndExp = 75,                   /* LAndExp  */
  YYSYMBOL_EqExp = 76,                     /* EqExp  */
  YYSYMBOL_RelExp = 77,                    /* RelExp  */
  YYSYMBOL_AddExp = 78,                    /* AddExp  */
  YYSYMBOL_MulExp = 79,                    /* MulExp  */
  YYSYMBOL_UnaryExp = 80,                  /* UnaryExp  */
  YYSYMBOL_UnaryOp = 81,                   /* UnaryOp  */
  YYSYMBOL_PrimaryExp = 82,                /* PrimaryExp  */
  YYSYMBOL_Number = 83,                    /* Number  */
  YYSYMBOL_LVal = 84,                      /* LVal  */
  YYSYMBOL_LValDims = 85,                  /* LValDims  */
  YYSYMBOL_FuncRParams = 86,               /* FuncRParams  */
  YYSYMBOL_FuncRParamsList = 87            /* FuncRParamsList  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




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

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
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
typedef yytype_uint8 yy_state_t;

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
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
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

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

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
#define YYFINAL  10
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   220

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  48
/* YYNRULES -- Number of rules.  */
#define YYNRULES  106
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  189

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   294


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

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
      35,    36,    37,    38,    39
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    39,    39,    40,    44,    45,    46,    46,    51,    51,
      53,    53,    58,    59,    63,    64,    72,    76,    77,    81,
      85,    86,    91,    92,    93,    97,    98,   107,   111,   112,
     116,   117,   121,   122,   127,   128,   129,   133,   134,   142,
     143,   147,   148,   152,   153,   158,   159,   167,   171,   172,
     176,   177,   178,   186,   187,   188,   189,   190,   192,   194,
     195,   196,   197,   198,   199,   204,   208,   209,   217,   221,
     225,   230,   231,   236,   237,   242,   243,   244,   249,   250,
     251,   252,   253,   258,   259,   260,   265,   266,   267,   268,
     273,   274,   275,   276,   280,   281,   282,   287,   288,   289,
     293,   298,   302,   303,   308,   312,   313
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "CONSTTK", "INTTK",
  "VOIDTK", "IFTK", "ELSETK", "WHILETK", "BREAKTK", "CONTINUETK",
  "RETURNTK", "PRINTFTK", "IDENFR", "INTCON", "STRCON", "LEQ", "GEQ",
  "EQL", "NEQ", "AND", "OR", "PLUS", "MINU", "MULT", "DIV", "MOD", "NOT",
  "LSS", "GRE", "ASSIGN", "SEMICN", "COMMA", "LPARENT", "RPARENT",
  "LBRACK", "RBRACK", "LBRACE", "RBRACE", "LOWER_THAN_ELSE", "$accept",
  "CompUnit", "CompUnitItem", "$@1", "IntIdentRest", "$@2", "$@3",
  "VarDefSuffix", "VarDefListTail", "ConstDecl", "ConstDefList",
  "ConstDef", "ConstDims", "ConstInitVal", "ConstInitValList", "VarDecl",
  "VarDefList", "VarDef", "VarDefDims", "InitVal", "InitValList",
  "FuncFParamsOpt", "FuncFParamsList", "FuncFParam", "FuncFParamDims",
  "Block", "BlockItems", "BlockItem", "Stmt", "FormatString",
  "PrintfArgsTail", "Exp", "Cond", "ConstExp", "LOrExp", "LAndExp",
  "EqExp", "RelExp", "AddExp", "MulExp", "UnaryExp", "UnaryOp",
  "PrimaryExp", "Number", "LVal", "LValDims", "FuncRParams",
  "FuncRParamsList", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-149)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      70,    23,    34,  -149,    65,  -149,  -149,    48,    55,    88,
    -149,  -149,  -149,    52,  -149,  -149,  -149,  -149,   -20,    72,
      -6,  -149,    48,   108,  -149,   140,   175,   108,   142,   175,
    -149,    97,    82,    94,  -149,    68,    95,  -149,  -149,  -149,
    -149,   175,    49,  -149,  -149,    81,    54,  -149,   175,  -149,
    -149,  -149,    96,    81,   102,   100,  -149,  -149,   103,   105,
     104,   108,  -149,   130,   158,   122,   124,  -149,  -149,   -24,
     175,   175,   175,   175,   175,  -149,  -149,   104,  -149,  -149,
      14,  -149,   123,  -149,  -149,  -149,  -149,  -149,  -149,  -149,
     126,   129,   175,  -149,   140,  -149,    54,    54,  -149,  -149,
    -149,  -149,   142,  -149,  -149,    22,    13,  -149,   175,   132,
    -149,  -149,   131,   130,   137,   141,   145,   147,   173,   149,
    -149,  -149,  -149,  -149,  -149,  -149,  -149,   152,   154,   140,
    -149,  -149,   175,    77,  -149,   175,   175,  -149,  -149,  -149,
     159,   178,  -149,   175,  -149,   163,  -149,   130,   160,   180,
     183,   106,   -10,    81,   171,  -149,  -149,  -149,   176,  -149,
    -149,    84,   175,   175,   175,   175,   175,   175,   175,   175,
      84,    24,  -149,   202,   183,   106,   -10,   -10,    81,    81,
      81,    81,  -149,   175,   179,    84,  -149,  -149,  -149
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     6,     0,     2,     4,     0,    32,     0,
       1,     3,    20,     0,    17,    10,     5,     8,    12,     0,
       0,    16,     0,    39,    14,     0,     0,    39,     0,     0,
      18,     0,     0,    40,    41,     0,   102,   100,    94,    95,
      96,     0,     0,    13,    34,    68,    83,    86,     0,    90,
      99,    98,     0,    70,     0,     0,    19,    22,     0,    43,
       0,     0,     9,     0,     0,   101,     0,    35,    37,     0,
       0,     0,     0,     0,     0,    93,    33,     0,    23,    25,
       0,    21,     0,    48,    11,    42,    32,    15,    91,   105,
       0,   104,     0,    97,     0,    36,    84,    85,    87,    88,
      89,     7,     0,    24,    45,     0,    30,    92,     0,     0,
      38,    26,    44,     0,     0,     0,     0,     0,     0,     0,
      55,    47,    50,    51,    56,    49,    52,     0,    98,     0,
     106,   103,     0,     0,    28,     0,     0,    60,    61,    62,
       0,     0,    54,     0,    31,     0,    27,     0,     0,    69,
      71,    73,    75,    78,     0,    63,    65,    66,     0,    46,
      29,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,    58,    72,    74,    76,    77,    81,    82,
      79,    80,    59,     0,     0,     0,    67,    64,    57
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -149,  -149,   207,  -149,  -149,  -149,  -149,  -149,  -149,   107,
    -149,   191,  -149,   -52,  -149,  -149,  -149,  -108,   128,   -40,
    -149,   188,  -149,   155,  -149,   -56,  -149,  -149,  -148,  -149,
    -149,   -41,    83,    -9,  -149,    56,    57,   -34,   -17,    64,
     -32,  -149,  -149,  -149,  -104,  -149,  -149,  -149
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     4,     5,     9,    16,    24,    23,    17,    35,     6,
      13,    14,    20,    56,    80,   123,   133,    87,    18,    43,
      69,    32,    33,    34,   112,   124,   105,   125,   126,   157,
     171,    44,   148,    57,   149,   150,   151,   152,    45,    46,
      47,    48,    49,    50,    51,    65,    90,    91
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      66,   128,    68,    79,    84,   134,   166,   167,    94,    53,
      25,    53,    53,   173,    95,    26,    75,    52,   168,   169,
      58,   101,   182,    89,    28,     1,   113,     7,   114,    29,
     115,   116,   117,   118,   119,    36,    37,   188,    53,   160,
      98,    99,   100,   129,    38,    39,   102,     8,    26,    40,
     111,   109,   103,   120,   110,    41,   183,   128,   184,    83,
     121,    12,    36,    37,   127,    10,   128,   130,     1,     2,
       3,    38,    39,     1,     2,     3,    40,   140,    72,    73,
      74,   128,    41,    21,    22,    53,    42,    67,    15,   144,
     114,   145,   115,   116,   117,   118,   119,    36,    37,    62,
      63,    19,   158,    70,    71,    27,    38,    39,   146,   147,
      59,    40,    31,    36,    37,   120,    60,    41,   153,   153,
     127,    83,    38,    39,   164,   165,    61,    40,    64,   127,
     176,   177,    76,    41,    96,    97,    77,    55,    78,    81,
      82,    83,   186,    86,   127,   153,   153,   153,   153,   178,
     179,   180,   181,    36,    37,    36,    37,    92,    93,   104,
     107,   108,    38,    39,    38,    39,   132,    40,   131,    40,
     135,    36,    37,    41,   136,    41,   137,    42,   138,    55,
      38,    39,   141,   142,   143,    40,    36,    37,    36,    37,
     155,    41,    88,   156,   161,    38,    39,    38,    39,   159,
      40,   162,    40,   163,   139,   170,    41,   172,    41,   185,
     187,    11,   122,    30,   106,    54,    85,     0,   174,   154,
     175
};

static const yytype_int16 yycheck[] =
{
      41,   105,    42,    55,    60,   113,    16,    17,    32,    26,
      30,    28,    29,   161,    38,    35,    48,    26,    28,    29,
      29,    77,   170,    64,    30,     3,     4,     4,     6,    35,
       8,     9,    10,    11,    12,    13,    14,   185,    55,   147,
      72,    73,    74,    30,    22,    23,    32,    13,    35,    27,
     102,    92,    38,    31,    94,    33,    32,   161,    34,    37,
      38,    13,    13,    14,   105,     0,   170,   108,     3,     4,
       5,    22,    23,     3,     4,     5,    27,   118,    24,    25,
      26,   185,    33,    31,    32,   102,    37,    38,    33,   129,
       6,   132,     8,     9,    10,    11,    12,    13,    14,    31,
      32,    13,   143,    22,    23,    33,    22,    23,    31,    32,
      13,    27,     4,    13,    14,    31,    34,    33,   135,   136,
     161,    37,    22,    23,    18,    19,    32,    27,    33,   170,
     164,   165,    36,    33,    70,    71,    34,    37,    38,    36,
      35,    37,   183,    13,   185,   162,   163,   164,   165,   166,
     167,   168,   169,    13,    14,    13,    14,    35,    34,    36,
      34,    32,    22,    23,    22,    23,    35,    27,    36,    27,
      33,    13,    14,    33,    33,    33,    31,    37,    31,    37,
      22,    23,    33,    31,    30,    27,    13,    14,    13,    14,
      31,    33,    34,    15,    34,    22,    23,    22,    23,    36,
      27,    21,    27,    20,    31,    34,    33,    31,    33,     7,
      31,     4,   105,    22,    86,    27,    61,    -1,   162,   136,
     163
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,    41,    42,    49,     4,    13,    43,
       0,    42,    13,    50,    51,    33,    44,    47,    58,    13,
      52,    31,    32,    46,    45,    30,    35,    33,    30,    35,
      51,     4,    61,    62,    63,    48,    13,    14,    22,    23,
      27,    33,    37,    59,    71,    78,    79,    80,    81,    82,
      83,    84,    73,    78,    61,    37,    53,    73,    73,    13,
      34,    32,    31,    32,    33,    85,    71,    38,    59,    60,
      22,    23,    24,    25,    26,    80,    36,    34,    38,    53,
      54,    36,    35,    37,    65,    63,    13,    57,    34,    71,
      86,    87,    35,    34,    32,    38,    79,    79,    80,    80,
      80,    65,    32,    38,    36,    66,    58,    34,    32,    71,
      59,    53,    64,     4,     6,     8,     9,    10,    11,    12,
      31,    38,    49,    55,    65,    67,    68,    71,    84,    30,
      71,    36,    35,    56,    57,    33,    33,    31,    31,    31,
      71,    33,    31,    30,    59,    71,    31,    32,    72,    74,
      75,    76,    77,    78,    72,    31,    15,    69,    71,    36,
      57,    34,    21,    20,    18,    19,    16,    17,    28,    29,
      34,    70,    31,    68,    75,    76,    77,    77,    78,    78,
      78,    78,    68,    32,    34,     7,    71,    31,    68
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    40,    41,    41,    42,    42,    43,    42,    45,    44,
      46,    44,    47,    47,    48,    48,    49,    50,    50,    51,
      52,    52,    53,    53,    53,    54,    54,    55,    56,    56,
      57,    57,    58,    58,    59,    59,    59,    60,    60,    61,
      61,    62,    62,    63,    63,    64,    64,    65,    66,    66,
      67,    67,    67,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    69,    70,    70,    71,    72,
      73,    74,    74,    75,    75,    76,    76,    76,    77,    77,
      77,    77,    77,    78,    78,    78,    79,    79,    79,    79,
      80,    80,    80,    80,    81,    81,    81,    82,    82,    82,
      83,    84,    85,    85,    86,    87,    87
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     3,     0,     7,     0,     4,
       0,     5,     1,     3,     0,     3,     4,     1,     3,     4,
       0,     4,     1,     2,     3,     1,     3,     3,     1,     3,
       2,     4,     0,     4,     1,     2,     3,     1,     3,     0,
       1,     1,     3,     2,     5,     0,     4,     3,     0,     2,
       1,     1,     1,     4,     2,     1,     1,     7,     5,     5,
       2,     2,     2,     3,     6,     1,     0,     3,     1,     1,
       1,     1,     3,     1,     3,     1,     3,     3,     1,     3,
       3,     3,     3,     1,     3,     3,     1,     3,     3,     3,
       1,     3,     4,     2,     1,     1,     1,     3,     1,     1,
       1,     2,     0,     4,     1,     1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


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
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
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
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

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
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
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
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  case 2: /* CompUnit: CompUnitItem  */
#line 39 "parser.y"
                                            { P("<CompUnit>"); }
#line 1299 "parser.tab.c"
    break;

  case 3: /* CompUnit: CompUnit CompUnitItem  */
#line 40 "parser.y"
                                            { P("<CompUnit>"); }
#line 1305 "parser.tab.c"
    break;

  case 6: /* $@1: %empty  */
#line 46 "parser.y"
             { P("<FuncType>"); }
#line 1311 "parser.tab.c"
    break;

  case 7: /* CompUnitItem: VOIDTK $@1 IDENFR LPARENT FuncFParamsOpt RPARENT Block  */
#line 47 "parser.y"
                                            { P("<FuncDef>"); }
#line 1317 "parser.tab.c"
    break;

  case 8: /* $@2: %empty  */
#line 51 "parser.y"
                   { P("<VarDef>"); }
#line 1323 "parser.tab.c"
    break;

  case 9: /* IntIdentRest: VarDefSuffix $@2 VarDefListTail SEMICN  */
#line 52 "parser.y"
                                            { P("<VarDecl>"); }
#line 1329 "parser.tab.c"
    break;

  case 10: /* $@3: %empty  */
#line 53 "parser.y"
              { P("<FuncType>"); }
#line 1335 "parser.tab.c"
    break;

  case 11: /* IntIdentRest: LPARENT $@3 FuncFParamsOpt RPARENT Block  */
#line 54 "parser.y"
                                            { P("<FuncDef>"); }
#line 1341 "parser.tab.c"
    break;

  case 16: /* ConstDecl: CONSTTK INTTK ConstDefList SEMICN  */
#line 72 "parser.y"
                                            { P("<ConstDecl>"); }
#line 1347 "parser.tab.c"
    break;

  case 19: /* ConstDef: IDENFR ConstDims ASSIGN ConstInitVal  */
#line 81 "parser.y"
                                            { P("<ConstDef>"); }
#line 1353 "parser.tab.c"
    break;

  case 22: /* ConstInitVal: ConstExp  */
#line 91 "parser.y"
                                            { P("<ConstInitVal>"); }
#line 1359 "parser.tab.c"
    break;

  case 23: /* ConstInitVal: LBRACE RBRACE  */
#line 92 "parser.y"
                                            { P("<ConstInitVal>"); }
#line 1365 "parser.tab.c"
    break;

  case 24: /* ConstInitVal: LBRACE ConstInitValList RBRACE  */
#line 93 "parser.y"
                                            { P("<ConstInitVal>"); }
#line 1371 "parser.tab.c"
    break;

  case 27: /* VarDecl: INTTK VarDefList SEMICN  */
#line 107 "parser.y"
                                           { P("<VarDecl>"); }
#line 1377 "parser.tab.c"
    break;

  case 30: /* VarDef: IDENFR VarDefDims  */
#line 116 "parser.y"
                                            { P("<VarDef>"); }
#line 1383 "parser.tab.c"
    break;

  case 31: /* VarDef: IDENFR VarDefDims ASSIGN InitVal  */
#line 117 "parser.y"
                                            { P("<VarDef>"); }
#line 1389 "parser.tab.c"
    break;

  case 34: /* InitVal: Exp  */
#line 127 "parser.y"
                                            { P("<InitVal>"); }
#line 1395 "parser.tab.c"
    break;

  case 35: /* InitVal: LBRACE RBRACE  */
#line 128 "parser.y"
                                            { P("<InitVal>"); }
#line 1401 "parser.tab.c"
    break;

  case 36: /* InitVal: LBRACE InitValList RBRACE  */
#line 129 "parser.y"
                                            { P("<InitVal>"); }
#line 1407 "parser.tab.c"
    break;

  case 40: /* FuncFParamsOpt: FuncFParamsList  */
#line 143 "parser.y"
                                            { P("<FuncFParams>"); }
#line 1413 "parser.tab.c"
    break;

  case 43: /* FuncFParam: INTTK IDENFR  */
#line 152 "parser.y"
                                            { P("<FuncFParam>"); }
#line 1419 "parser.tab.c"
    break;

  case 44: /* FuncFParam: INTTK IDENFR LBRACK RBRACK FuncFParamDims  */
#line 154 "parser.y"
                                            { P("<FuncFParam>"); }
#line 1425 "parser.tab.c"
    break;

  case 47: /* Block: LBRACE BlockItems RBRACE  */
#line 167 "parser.y"
                                            { P("<Block>"); }
#line 1431 "parser.tab.c"
    break;

  case 53: /* Stmt: LVal ASSIGN Exp SEMICN  */
#line 186 "parser.y"
                                            { P("<Stmt>"); }
#line 1437 "parser.tab.c"
    break;

  case 54: /* Stmt: Exp SEMICN  */
#line 187 "parser.y"
                                            { P("<Stmt>"); }
#line 1443 "parser.tab.c"
    break;

  case 55: /* Stmt: SEMICN  */
#line 188 "parser.y"
                                            { P("<Stmt>"); }
#line 1449 "parser.tab.c"
    break;

  case 56: /* Stmt: Block  */
#line 189 "parser.y"
                                            { P("<Stmt>"); }
#line 1455 "parser.tab.c"
    break;

  case 57: /* Stmt: IFTK LPARENT Cond RPARENT Stmt ELSETK Stmt  */
#line 191 "parser.y"
                                            { P("<Stmt>"); }
#line 1461 "parser.tab.c"
    break;

  case 58: /* Stmt: IFTK LPARENT Cond RPARENT Stmt  */
#line 193 "parser.y"
                                            { P("<Stmt>"); }
#line 1467 "parser.tab.c"
    break;

  case 59: /* Stmt: WHILETK LPARENT Cond RPARENT Stmt  */
#line 194 "parser.y"
                                            { P("<Stmt>"); }
#line 1473 "parser.tab.c"
    break;

  case 60: /* Stmt: BREAKTK SEMICN  */
#line 195 "parser.y"
                                            { P("<Stmt>"); }
#line 1479 "parser.tab.c"
    break;

  case 61: /* Stmt: CONTINUETK SEMICN  */
#line 196 "parser.y"
                                            { P("<Stmt>"); }
#line 1485 "parser.tab.c"
    break;

  case 62: /* Stmt: RETURNTK SEMICN  */
#line 197 "parser.y"
                                            { P("<Stmt>"); }
#line 1491 "parser.tab.c"
    break;

  case 63: /* Stmt: RETURNTK Exp SEMICN  */
#line 198 "parser.y"
                                            { P("<Stmt>"); }
#line 1497 "parser.tab.c"
    break;

  case 64: /* Stmt: PRINTFTK LPARENT FormatString PrintfArgsTail RPARENT SEMICN  */
#line 200 "parser.y"
                                            { P("<Stmt>"); }
#line 1503 "parser.tab.c"
    break;

  case 65: /* FormatString: STRCON  */
#line 204 "parser.y"
                                            { P("<FormatString>"); }
#line 1509 "parser.tab.c"
    break;

  case 68: /* Exp: AddExp  */
#line 217 "parser.y"
                                            { P("<Exp>"); }
#line 1515 "parser.tab.c"
    break;

  case 69: /* Cond: LOrExp  */
#line 221 "parser.y"
                                            { P("<Cond>"); }
#line 1521 "parser.tab.c"
    break;

  case 70: /* ConstExp: AddExp  */
#line 225 "parser.y"
                                            { P("<ConstExp>"); }
#line 1527 "parser.tab.c"
    break;

  case 71: /* LOrExp: LAndExp  */
#line 230 "parser.y"
                                            { P("<LOrExp>"); }
#line 1533 "parser.tab.c"
    break;

  case 72: /* LOrExp: LOrExp OR LAndExp  */
#line 231 "parser.y"
                                            { P("<LOrExp>"); }
#line 1539 "parser.tab.c"
    break;

  case 73: /* LAndExp: EqExp  */
#line 236 "parser.y"
                                            { P("<LAndExp>"); }
#line 1545 "parser.tab.c"
    break;

  case 74: /* LAndExp: LAndExp AND EqExp  */
#line 237 "parser.y"
                                            { P("<LAndExp>"); }
#line 1551 "parser.tab.c"
    break;

  case 75: /* EqExp: RelExp  */
#line 242 "parser.y"
                                            { P("<EqExp>"); }
#line 1557 "parser.tab.c"
    break;

  case 76: /* EqExp: EqExp EQL RelExp  */
#line 243 "parser.y"
                                            { P("<EqExp>"); }
#line 1563 "parser.tab.c"
    break;

  case 77: /* EqExp: EqExp NEQ RelExp  */
#line 244 "parser.y"
                                            { P("<EqExp>"); }
#line 1569 "parser.tab.c"
    break;

  case 78: /* RelExp: AddExp  */
#line 249 "parser.y"
                                            { P("<RelExp>"); }
#line 1575 "parser.tab.c"
    break;

  case 79: /* RelExp: RelExp LSS AddExp  */
#line 250 "parser.y"
                                            { P("<RelExp>"); }
#line 1581 "parser.tab.c"
    break;

  case 80: /* RelExp: RelExp GRE AddExp  */
#line 251 "parser.y"
                                            { P("<RelExp>"); }
#line 1587 "parser.tab.c"
    break;

  case 81: /* RelExp: RelExp LEQ AddExp  */
#line 252 "parser.y"
                                            { P("<RelExp>"); }
#line 1593 "parser.tab.c"
    break;

  case 82: /* RelExp: RelExp GEQ AddExp  */
#line 253 "parser.y"
                                            { P("<RelExp>"); }
#line 1599 "parser.tab.c"
    break;

  case 83: /* AddExp: MulExp  */
#line 258 "parser.y"
                                            { P("<AddExp>"); }
#line 1605 "parser.tab.c"
    break;

  case 84: /* AddExp: AddExp PLUS MulExp  */
#line 259 "parser.y"
                                            { P("<AddExp>"); }
#line 1611 "parser.tab.c"
    break;

  case 85: /* AddExp: AddExp MINU MulExp  */
#line 260 "parser.y"
                                            { P("<AddExp>"); }
#line 1617 "parser.tab.c"
    break;

  case 86: /* MulExp: UnaryExp  */
#line 265 "parser.y"
                                            { P("<MulExp>"); }
#line 1623 "parser.tab.c"
    break;

  case 87: /* MulExp: MulExp MULT UnaryExp  */
#line 266 "parser.y"
                                            { P("<MulExp>"); }
#line 1629 "parser.tab.c"
    break;

  case 88: /* MulExp: MulExp DIV UnaryExp  */
#line 267 "parser.y"
                                            { P("<MulExp>"); }
#line 1635 "parser.tab.c"
    break;

  case 89: /* MulExp: MulExp MOD UnaryExp  */
#line 268 "parser.y"
                                            { P("<MulExp>"); }
#line 1641 "parser.tab.c"
    break;

  case 90: /* UnaryExp: PrimaryExp  */
#line 273 "parser.y"
                                            { P("<UnaryExp>"); }
#line 1647 "parser.tab.c"
    break;

  case 91: /* UnaryExp: IDENFR LPARENT RPARENT  */
#line 274 "parser.y"
                                            { P("<UnaryExp>"); }
#line 1653 "parser.tab.c"
    break;

  case 92: /* UnaryExp: IDENFR LPARENT FuncRParams RPARENT  */
#line 275 "parser.y"
                                            { P("<UnaryExp>"); }
#line 1659 "parser.tab.c"
    break;

  case 93: /* UnaryExp: UnaryOp UnaryExp  */
#line 276 "parser.y"
                                            { P("<UnaryExp>"); }
#line 1665 "parser.tab.c"
    break;

  case 94: /* UnaryOp: PLUS  */
#line 280 "parser.y"
                                            { P("<UnaryOp>"); }
#line 1671 "parser.tab.c"
    break;

  case 95: /* UnaryOp: MINU  */
#line 281 "parser.y"
                                            { P("<UnaryOp>"); }
#line 1677 "parser.tab.c"
    break;

  case 96: /* UnaryOp: NOT  */
#line 282 "parser.y"
                                            { P("<UnaryOp>"); }
#line 1683 "parser.tab.c"
    break;

  case 97: /* PrimaryExp: LPARENT Exp RPARENT  */
#line 287 "parser.y"
                                            { P("<PrimaryExp>"); }
#line 1689 "parser.tab.c"
    break;

  case 98: /* PrimaryExp: LVal  */
#line 288 "parser.y"
                                            { P("<PrimaryExp>"); }
#line 1695 "parser.tab.c"
    break;

  case 99: /* PrimaryExp: Number  */
#line 289 "parser.y"
                                            { P("<PrimaryExp>"); }
#line 1701 "parser.tab.c"
    break;

  case 100: /* Number: INTCON  */
#line 293 "parser.y"
                                            { P("<Number>"); }
#line 1707 "parser.tab.c"
    break;

  case 101: /* LVal: IDENFR LValDims  */
#line 298 "parser.y"
                                            { P("<LVal>"); }
#line 1713 "parser.tab.c"
    break;

  case 104: /* FuncRParams: FuncRParamsList  */
#line 308 "parser.y"
                                            { P("<FuncRParams>"); }
#line 1719 "parser.tab.c"
    break;


#line 1723 "parser.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

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
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
                      yytoken, &yylval);
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
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 316 "parser.y"


void yyerror(const char* s) {
    fprintf(stderr, "Parse error: %s\n", s);
}

int main() {
    yyin = fopen("testfile.txt", "r");
    if (!yyin) {
        fprintf(stderr, "Cannot open testfile.txt\n");
        return 1;
    }
    fout = fopen("output.txt", "w");
    if (!fout) {
        fprintf(stderr, "Cannot open output.txt\n");
        return 1;
    }
    yyparse();
    fclose(fout);
    fclose(yyin);
    return 0;
}
