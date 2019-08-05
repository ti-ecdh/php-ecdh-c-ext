/*
 @author : Chick.LU
 @date   : 2019.08.04
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_ecdh.h"
#include "openssl/ssl.h"

/* If you declare any globals in php_ecdh.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(ecdh)
*/

/* True global resources - no need for thread safety here */
static int le_ecdh;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("ecdh.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_ecdh_globals, ecdh_globals)
    STD_PHP_INI_ENTRY("ecdh.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_ecdh_globals, ecdh_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_ecdh_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION( ecdh_init )
{
  //int builtin_curve_number;
  //EC_builtin_curve * builtin_curve;
  // 初始化变量
  int              ret;
  EC_KEY           * ecdh_key;
  const EC_GROUP   * group;
  const EC_POINT   * point;
  unsigned char    * public_key_hex;
  int              public_key_length;
  // 选择NID_X9_62_prime256v1椭圆曲线，要和客户端保持一致，选择同一条曲线.
  ecdh_key = EC_KEY_new_by_curve_name( NID_X9_62_prime256v1 );
  EC_KEY_generate_key( ecdh_key );
  // point就可以粗暴认为是公钥所在椭圆曲线的坐标点.
  point  = EC_KEY_get0_public_key( ecdh_key );
  // group则是存储了椭圆曲线各种信息的数据结构.
  group  = EC_KEY_get0_group( ecdh_key );
  // 将公钥取出来.
  //public_key_length = EC_POINT_point2oct( group, point, POINT_CONVERSION_COMPRESSED, NULL, 0, NULL );
  //public_key        = ( unsigned char * )malloc( public_key_length );
  public_key_hex = EC_POINT_point2hex( group, point, POINT_CONVERSION_COMPRESSED, NULL );
  //printf( "%s\n", public_key_hex );
  // 需要把 public_key 返回给调用方.
  RETURN_STRING( public_key_hex )
}
PHP_FUNCTION( ecdh_compute_share_key )
{
  char   * client_public_key_hex; 
  size_t client_public_key_hex_length; 
  //strg = strpprintf( 0, "dh compute key." );
  ZEND_PARSE_PARAMETERS_START( 1, 1 )
  Z_PARAM_STRING( client_public_key_hex, client_public_key_hex_length )
  ZEND_PARSE_PARAMETERS_END();
  EC_KEY         * ecdh_key; 
  EC_POINT       * point;
  const EC_GROUP * group;
  ecdh_key = EC_KEY_new_by_curve_name( NID_X9_62_prime256v1 );
  EC_KEY_generate_key( ecdh_key );
  group = EC_KEY_get0_group( ecdh_key );
  // 生成一个新的空点.
  point = EC_POINT_new( group ); 
  // 将参数中客户端传来的参数：public-key 转为椭圆曲线上的上面初始化的那个点.
  point = EC_POINT_hex2point( group, client_public_key_hex, point, NULL ); 
  //printf( "%p\n", point ); 
  //if ( 0 != EC_POINT_cmp( group, point1, new_point, NULL ) ) {
    //return -1;
  //}
  /*
  int ECDH_compute_key(
    void *out, size_t outlen, const EC_POINT *pub_key, EC_KEY *priv_key,
    void *( *KDF )( const void *in, size_t inlen, void *out, size_t *outlen )
  )
  */
  int share_key_length;
  share_key_length = EC_GROUP_get_degree( EC_KEY_get0_group( ecdh_key ) );
  share_key_length = ( share_key_length + 7 ) / 8;
  char * share_key = ( unsigned char * )malloc( share_key_length );
  ECDH_compute_key( share_key, share_key_length, point, ecdh_key, NULL );
  //dump( ( const char * )share_key, share_key_length );
  //printf( "%s\n", share_key );
  printf( "share key : " );
  for ( int i = 0; i < 100; i++ ) {
    printf( "%d", *( ( unsigned char * )share_key + i ) );
  }
  printf( "\n" );
  RETURN_STRING( "xxoo" )
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_ecdh_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_ecdh_init_globals(zend_ecdh_globals *ecdh_globals)
{
	ecdh_globals->global_value = 0;
	ecdh_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(ecdh)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(ecdh)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(ecdh)
{
#if defined(COMPILE_DL_ECDH) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(ecdh)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(ecdh)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "ecdh support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ ecdh_functions[]
 *
 * Every user visible function must have an entry in ecdh_functions[].
 */
const zend_function_entry ecdh_functions[] = {
	PHP_FE(ecdh_init,	NULL)
	PHP_FE(ecdh_compute_share_key,	NULL)
	PHP_FE_END	/* Must be the last line in ecdh_functions[] */
};
/* }}} */

/* {{{ ecdh_module_entry
 */
zend_module_entry ecdh_module_entry = {
	STANDARD_MODULE_HEADER,
	"ecdh",
	ecdh_functions,
	PHP_MINIT(ecdh),
	PHP_MSHUTDOWN(ecdh),
	PHP_RINIT(ecdh),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(ecdh),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(ecdh),
	PHP_ECDH_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_ECDH
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(ecdh)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
