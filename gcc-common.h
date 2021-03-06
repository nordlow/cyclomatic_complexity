#ifndef GCC_COMMON_H_INCLUDED
#define GCC_COMMON_H_INCLUDED

#include "plugin.h"
#include "bversion.h"
#include "plugin-version.h"
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "line-map.h"
#include "input.h"
#include "tree.h"

#include "tree-inline.h"
#include "version.h"
#include "rtl.h"
#include "tm_p.h"
#include "flags.h"
//#include "insn-attr.h"
//#include "insn-config.h"
//#include "insn-flags.h"
#include "hard-reg-set.h"
//#include "recog.h"
#include "output.h"
#include "except.h"
#include "function.h"
#include "toplev.h"
//#include "expr.h"
#include "basic-block.h"
#include "intl.h"
#include "ggc.h"
//#include "regs.h"
#include "timevar.h"

#include "params.h"
#include "pointer-set.h"
#include "emit-rtl.h"
//#include "reload.h"
//#include "ira.h"
//#include "dwarf2asm.h"
#include "debug.h"
#include "target.h"
#include "langhooks.h"
#include "cfgloop.h"
//#include "hosthooks.h"
#include "cgraph.h"
#include "opts.h"
//#include "coverage.h"
//#include "value-prof.h"

#if BUILDING_GCC_VERSION >= 4007
#include "tree-pretty-print.h"
#include "gimple-pretty-print.h"
#include "c-tree.h"
//#include "alloc-pool.h"
#endif

#if BUILDING_GCC_VERSION <= 4008
#include "tree-flow.h"
#endif

#include "diagnostic.h"
//#include "tree-diagnostic.h"
#include "tree-dump.h"
#include "tree-pass.h"
//#include "df.h"
#include "predict.h"
//#include "lto-streamer.h"
#include "ipa-utils.h"

#if BUILDING_GCC_VERSION >= 4009
#include "varasm.h"
#include "stor-layout.h"
#include "internal-fn.h"
#include "gimple-expr.h"
//#include "diagnostic-color.h"
#include "context.h"
#include "tree-ssa-alias.h"
#include "stringpool.h"
#include "tree-ssanames.h"
#include "print-tree.h"
#include "tree-eh.h"
#endif

#include "gimple.h"

#if BUILDING_GCC_VERSION >= 4009
#include "tree-ssa-operands.h"
#include "tree-phinodes.h"
#include "tree-cfg.h"
#include "gimple-iterator.h"
#include "gimple-ssa.h"
#include "ssa-iterators.h"
#endif

//#include "expr.h" where are you...
extern rtx emit_move_insn(rtx x, rtx y);

#define __unused __attribute__((__unused__))

#define DECL_NAME_POINTER(node) IDENTIFIER_POINTER(DECL_NAME(node))
#define DECL_NAME_LENGTH(node) IDENTIFIER_LENGTH(DECL_NAME(node))

#if BUILDING_GCC_VERSION == 4005
#define FOR_EACH_LOCAL_DECL(FUN, I, D) for (tree vars = (FUN)->local_decls; vars && (D = TREE_VALUE(vars)); vars = TREE_CHAIN(vars), I)
#define DECL_CHAIN(NODE) (TREE_CHAIN(DECL_MINIMAL_CHECK(NODE)))
#define FOR_EACH_VEC_ELT(T, V, I, P) for (I = 0; VEC_iterate(T, (V), (I), (P)); ++(I))

static inline bool gimple_call_builtin_p(gimple stmt, enum built_in_function code)
{
	tree fndecl;

	if (!is_gimple_call(stmt))
		return false;
	fndecl = gimple_call_fndecl(stmt);
	if (!fndecl || DECL_BUILT_IN_CLASS(fndecl) != BUILT_IN_NORMAL)
		return false;
//	print_node(stderr, "pax", fndecl, 4);
	return DECL_FUNCTION_CODE(fndecl) == code;
}

static inline bool is_simple_builtin(tree decl)
{
	if (decl && DECL_BUILT_IN_CLASS(decl) != BUILT_IN_NORMAL)
		return false;

	switch (DECL_FUNCTION_CODE(decl)) {
	/* Builtins that expand to constants. */
	case BUILT_IN_CONSTANT_P:
	case BUILT_IN_EXPECT:
	case BUILT_IN_OBJECT_SIZE:
	case BUILT_IN_UNREACHABLE:
	/* Simple register moves or loads from stack. */
	case BUILT_IN_RETURN_ADDRESS:
	case BUILT_IN_EXTRACT_RETURN_ADDR:
	case BUILT_IN_FROB_RETURN_ADDR:
	case BUILT_IN_RETURN:
	case BUILT_IN_AGGREGATE_INCOMING_ADDRESS:
	case BUILT_IN_FRAME_ADDRESS:
	case BUILT_IN_VA_END:
	case BUILT_IN_STACK_SAVE:
	case BUILT_IN_STACK_RESTORE:
	/* Exception state returns or moves registers around. */
	case BUILT_IN_EH_FILTER:
	case BUILT_IN_EH_POINTER:
	case BUILT_IN_EH_COPY_VALUES:
	return true;

	default:
	return false;
	}
}
#endif

#if BUILDING_GCC_VERSION <= 4006
#define ANY_RETURN_P(rtx) (GET_CODE(rtx) == RETURN)
#define C_DECL_REGISTER(EXP) DECL_LANG_FLAG_4(EXP)

// should come from c-tree.h if only it were installed for gcc 4.5...
#define C_TYPE_FIELDS_READONLY(TYPE) TREE_LANG_FLAG_1(TYPE)

#define get_random_seed(noinit) ({						\
	unsigned HOST_WIDE_INT seed;						\
	sscanf(get_random_seed(noinit), "%" HOST_WIDE_INT_PRINT "x", &seed);	\
	seed * seed; })

static inline bool gimple_clobber_p(gimple s)
{
	return false;
}

static inline tree builtin_decl_implicit(enum built_in_function fncode)
{
	return implicit_built_in_decls[fncode];
}

static inline struct cgraph_node *cgraph_get_create_node(tree decl)
{
	struct cgraph_node *node = cgraph_get_node(decl);

	return node ? node : cgraph_node(decl);
}

static inline bool cgraph_function_with_gimple_body_p(struct cgraph_node *node)
{
	return node->analyzed && !node->thunk.thunk_p && !node->alias;
}

static inline struct cgraph_node *cgraph_first_function_with_gimple_body(void)
{
	struct cgraph_node *node;

	for (node = cgraph_nodes; node; node = node->next)
		if (cgraph_function_with_gimple_body_p(node))
			return node;
	return NULL;
}

static inline struct cgraph_node *cgraph_next_function_with_gimple_body(struct cgraph_node *node)
{
	for (node = node->next; node; node = node->next)
		if (cgraph_function_with_gimple_body_p(node))
			return node;
	return NULL;
}

#define FOR_EACH_FUNCTION_WITH_GIMPLE_BODY(node) \
	for ((node) = cgraph_first_function_with_gimple_body(); (node); \
		(node) = cgraph_next_function_with_gimple_body(node))
#endif

#if BUILDING_GCC_VERSION == 4006
extern void debug_gimple_stmt(gimple);
extern void debug_gimple_seq(gimple_seq);
extern void print_gimple_seq(FILE *, gimple_seq, int, int);
extern void print_gimple_stmt(FILE *, gimple, int, int);
extern void print_gimple_expr(FILE *, gimple, int, int);
extern void dump_gimple_stmt(pretty_printer *, gimple, int, int);
#endif

#if BUILDING_GCC_VERSION <= 4007
#define FOR_EACH_VARIABLE(node) for (node = varpool_nodes; node; node = node->next)

static inline bool gimple_store_p(gimple gs)
{
	tree lhs = gimple_get_lhs(gs);
	return lhs && !is_gimple_reg(lhs);
}
#endif

#if BUILDING_GCC_VERSION >= 4007
#define cgraph_create_edge(caller, callee, call_stmt, count, freq, nest) \
	cgraph_create_edge((caller), (callee), (call_stmt), (count), (freq))
#endif

#if BUILDING_GCC_VERSION <= 4008
#define ENTRY_BLOCK_PTR_FOR_FN(FN)	ENTRY_BLOCK_PTR_FOR_FUNCTION(FN)
#define EXIT_BLOCK_PTR_FOR_FN(FN)	EXIT_BLOCK_PTR_FOR_FUNCTION(FN)
#define basic_block_info_for_fn(FN)	((FN)->cfg->x_basic_block_info)
#define n_basic_blocks_for_fn(FN)	((FN)->cfg->x_n_basic_blocks)
#define n_edges_for_fn(FN)		((FN)->cfg->x_n_edges)
#define last_basic_block_for_fn(FN)	((FN)->cfg->x_last_basic_block)
#define label_to_block_map_for_fn(FN)	((FN)->cfg->x_label_to_block_map)
#define profile_status_for_fn(FN)	((FN)->cfg->x_profile_status)

static inline const char *get_tree_code_name(enum tree_code code)
{
	gcc_assert(code < MAX_TREE_CODES);
	return tree_code_name[code];
}

#define ipa_remove_stmt_references(cnode, stmt)
#endif

#if BUILDING_GCC_VERSION == 4008
#define NODE_DECL(node) node->symbol.decl
#else
#define NODE_DECL(node) node->decl
#endif

#if BUILDING_GCC_VERSION >= 4008
#define add_referenced_var(var)
#define mark_sym_for_renaming(var)
#define varpool_mark_needed_node(node)
#define TODO_dump_func 0
#define TODO_dump_cgraph 0
#endif

#if BUILDING_GCC_VERSION >= 4009
#define TODO_ggc_collect 0
#endif

#endif
