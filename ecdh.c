#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/ec.h>
#include <openssl/ecdh.h>
/*
 用了这么多年high-level级的openssl函数
 今天是第一次些原生openssl api
 openssl将aes、rsa、ecc、验签等安全加密什么的真是一锅端了
 */
int main( int argc, char * argv[] ) {
  /*----------- 首先接触几个openssl库的API ----------*/
  // 初始化一坨变量们
  int                builtin_curve_length;
  EC_builtin_curve * builtin_curve;
  EC_KEY           * ecdh_key;
  const EC_POINT * point;
  const EC_GROUP * group;
  unsigned char public_key[ 100 ];
  unsigned char shared_key[ 100 ];
  int   length;
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
  ecdh_key = EC_KEY_new_by_curve_name( builtin_curve[ 12 ].nid );
  EC_KEY_generate_key( ecdh_key );
  //printf( "%d\n", ecdh_key->enc_flag );
  point = EC_KEY_get0_public_key( ecdh_key ); 
  group = EC_KEY_get0_group( ecdh_key );
  length = EC_POINT_point2oct( group, point, POINT_CONVERSION_COMPRESSED, public_key, 100, NULL );  
  //printf( "%d\n", length );
  // 计算出对称密钥
  length = ECDH_compute_key( shared_key, 100, point, ecdh_key, NULL );
  //printf( "%d\n", length );
  //printf( "%s\n", shared_key );
  for ( int i = 0; i < length; i++ ) {
    printf( "%d", *( ( unsigned char * )shared_key + i ) );
  }
  printf( "\n" );
  return 0;
}
/*
// EC_builtin_curve
typedef struct {
    int nid;
    const char *comment;
} EC_builtin_curve;
// EC_GROUP
typedef struct ec_group_st {
    const EC_METHOD *meth;      //椭圆曲线运算函数 
    EC_POINT *generator;        //optional 
    BIGNUM *order, *cofactor;
    ... ...
    BIGNUM *field;
    int poly[6];
    BIGNUM *a, *b;
    ...
} EC_GROUP;
// EC_KEY
typedef struct ec_key_st {
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
