// Regression test: compiling a program with import/include through libjq
// must not abort when JQ_LIBRARY_PATH is unset or is not an array.
#include <stdio.h>
#include <stdlib.h>
#include <jq.h>

static int compile(const char *prog, int set_null) {
  jq_state *jq = jq_init();
  if (jq == NULL) exit(2);
  if (set_null)
    jq_set_attr(jq, jv_string("JQ_LIBRARY_PATH"), jv_null());
  // A "module not found" error (return 0) is fine; it must just not abort.
  int r = jq_compile(jq, prog);
  jq_teardown(&jq);
  return r;
}

int main(void) {
  int fail = 0;
  for (int set_null = 0; set_null < 2; set_null++) {
    if (compile("import \"nonexistent\" as a; .", set_null)) fail = 1;
    if (compile("include \"nonexistent\"; .", set_null)) fail = 1;
    if (compile("import \"nonexistent\" as $a; .", set_null)) fail = 1;
    if (compile("import \"nonexistent\" as a {search:\"./\"}; .", set_null)) fail = 1;
    if (!compile(".", set_null)) fail = 1;
  }
  return fail;
}
