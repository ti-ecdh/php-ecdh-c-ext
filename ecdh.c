#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/ec.h>
/*
 用了这么多年high-level级的openssl函数
 今天是第一次些原生openssl api
 openssl将aes、rsa、ecc、验签等安全加密什么的真是一锅端了
 */
int main( int argc, char * argv[] ) {
  // 首先接触几个openssl库的API
  int builtin_curve_length;
  EC_builtin_curve * builtin_curve;
  // 瞅瞅openssl内置了多少种椭圆.
  builtin_curve_length = EC_get_builtin_curves( NULL, 0 ); 
  // 然后我们将这些椭圆全部拿出来，保存到EC_builtin_curve指针中
  builtin_curve = ( EC_builtin_curve * )malloc( sizeof( EC_builtin_curve ) * builtin_curve_length ); 
  EC_get_builtin_curves( builtin_curve, builtin_curve_length );
  /*
  // 可以通过这个for循环来查看openssl内置的N种椭圆都是哪些
  for( int i = 0; i < builtin_curve_length; i++ ) {
    printf( "%d : %d\n", i, builtin_curve[ i ].nid );
  }
  */
  // 接着我们选择一种椭圆，利用这个椭圆生成一对用于非对称的公私钥
  return 0;
}

/*

typedef struct {
    int nid;
    const char *comment;
} EC_builtin_curve;

type struct ec_group_st {
    const EC_METHOD *meth;      //椭圆曲线运算函数 
    EC_POINT *generator;        //optional 
    BIGNUM *order, *cofactor;
    ... ...
    BIGNUM *field;
    int poly[6];
    BIGNUM *a, *b;
    ...
};

type struct ec_key_st {
    const EC_KEY_METHOD *meth;//计算函数
    ENGINE *engine;
    int version;
    EC_GROUP *group;//群
    EC_POINT *pub_key;//公钥
    BIGNUM *priv_key;//私钥
    unsigned int enc_flag;
    point_conversion_form_t conv_form;
    CRYPTO_REF_COUNT references;
    int flags;
    CRYPTO_EX_DATA ex_data;
    CRYPTO_RWLOCK *lock;
};

*/
