#include <stdlib.h>
#include <stdio.h>

#include "td.h"

// this is the class that's called below... change it to use something else...
#define MAIN_CLASS "xlang/java/Xlang"

// for java, first argument can be a classpath
int main(int argc, char *argv[])
{
#ifdef TD_HAS_JULIA
    // start julia
    td_env_t *jl = td_env_julia(".",
                                "/Applications/Julia-0.2.1.app/Contents/Resources/julia/bin");


    // call "sin" with one scalar argument
    td_val_t arg = { .tag = TD_DOUBLE, .double_val = 3.14 };
    td_val_t out;
    jl->invoke1(&out, "sin", &arg);

    printf("sin(3.14) = %g\n", td_double(&out));


    // call "norm" on a Float64 3-vector
    double v[] = { 1.0, 2.0, 3.0 };
    td_array_t a = { .data=v, .length=3, .eltype=TD_DOUBLE, .ndims=1 };
    td_val_t av = { .tag = TD_ARRAY, .object = &a };
    jl->invoke1(&out, "norm", &av);

    printf("norm([1.0,2.0,3.0]) = %g\n", td_double(&out));
#endif

#ifdef TD_HAS_PYTHON
    td_val_t out_py;
    td_env_t *py = td_env_python(".",
                                 "/Users/aterrel/workspace/opt/apps/anaconda/anaconda-1.9.1/anaconda/bin/python");
    py->invoke0(&out_py, "int");
    printf("int() = %d\n", td_int32(&out_py));

    td_val_t arg = { .tag = TD_INT32, .int32_val = 2 };

    py->invoke1(&out_py, "int", &arg);
    printf("int(2) = %d\n", td_int32(&out_py));
#endif

#ifdef TD_HAS_JAVA

    td_val_t out_java;

    char *classpath = "out";
    if (argc == 2) {
    	classpath = argv[1];
    }
    td_env_t *java_env = td_env_java(".",classpath,MAIN_CLASS);

    // tests!

    td_string_t str;
    str.length = 7;
    str.data = "Bueller";

    td_val_t arg;
    arg.tag = TD_UTF8; arg.object = &str;
    java_env->invoke1(&out_java, "strLen", &arg);
    printf("strLen(%s) = %d\n", (char *)((td_string_t *)arg.object)->data, td_int32(&out_java));

    arg.tag = TD_UTF8; arg.object = &str;
    java_env->invoke1(&out_java, "toUpper", &arg);
    printf("toUpper(%s) = %s\n", (char *)((td_string_t *)arg.object)->data, (char *)((td_string_t *)out_java.object)->data);

    td_array_t arr;
    arr.eltype = TD_UTF8;
    arr.length = 3;

    td_string_t str1 = { "one",3};
    td_string_t str2 = { "two",3};
    td_string_t str3 = { "three",5};
    //char * words[3] = {"one","two","three"};
    td_string_t * words[3] = {&str1,&str2,&str3};
    arr.data = words;
    arg.tag = TD_ARRAY; arg.object = &arr;
    java_env->invoke1(&out_java, "howManyArr", &arg);
    printf("howManyArr = %d\n",  td_int32(&out_java));

    if (1) return 0;

    // sum ints
   // td_array_t arr;
    arr.eltype = TD_INT32;
    arr.length = 3;
    int nums[3] = {1,2,3};
    arr.data = &nums;
    arg.tag = TD_ARRAY; arg.object = &arr;
    java_env->invoke1(&out_java, "sumArr", &arg);
    printf("sum = %d\n",  td_int32(&out_java));

    // sum doubles
    double dnums[3] = {3.14,3.14,3.14};
    arr.eltype = TD_DOUBLE;
    arr.data = &dnums;
    arg.tag = TD_ARRAY; arg.object = &arr;
    java_env->invoke1(&out_java, "sumDoubleArr", &arg);
    printf("sum = %f\n",  td_double(&out_java));

    java_env->invoke0(&out_java, "nextInt");
    printf("nextInt() = %d tag %d\n", td_int32(&out_java), td_typeof(&out_java));

    java_env->invoke0(&out_java, "nextBool");
    printf("nextBool() = %d\n", td_int32(&out_java));

    java_env->invoke0(&out_java, "nextDouble");
    printf("nextDouble() = %f\n", td_double(&out_java));

    arg.tag = TD_INT32; arg.double_val = 4;
    java_env->invoke1(&out_java, "sqr", &arg);
    printf("sqr(2) = %d\n", td_int32(&out_java));

    arg.tag = TD_DOUBLE; arg.double_val = 3.14159/2;
    java_env->invoke1(&out_java, "sin", &arg);
    printf("sin(%f) = %f\n", arg.double_val, td_double(&out_java));

    arg.tag = TD_INT32; arg.int32_val = 4;
    java_env->invoke1(&out_java, "isEven", &arg);
    printf("isEven(%d) = %d\n", arg.int32_val, td_uint32(&out_java));

    arg.tag = TD_INT32; arg.int32_val = 3;
    java_env->invoke1(&out_java, "isEven", &arg);
    printf("isEven(%d) = %d\n", arg.int32_val, td_uint32(&out_java));

    // error test cases -------------------------------------------

    // bad method name
    java_env->invoke0(&out_java, "unknownMethod");
    printf("unknownMethod() = %f\n", td_double(&out_java));

    // another error case - no sqr that takes a double
    arg.tag = TD_DOUBLE; arg.double_val = 3.14159/2;
    java_env->invoke1(&out_java, "sqr", &arg);
    printf("sqr(%f) = %f\n", arg.double_val, td_double(&out_java));
#endif

    return 0;
}
