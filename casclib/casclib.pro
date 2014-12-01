TEMPLATE = lib
CONFIG += staticlib

TARGET = casc

HEADERS += remote/src/CascCommon.h \
remote/src/CascLib.h \
remote/src/CascMndxRoot.h \
remote/src/CascPort.h \
remote/src/common/Common.h \
remote/src/common/FileStream.h \
remote/src/common/ListFile.h \
remote/src/common/Map.h \
remote/src/jenkins/lookup.h \
remote/src/libtomcrypt/src/headers/tomcrypt.h \
remote/src/libtomcrypt/src/headers/tomcrypt_argchk.h \
remote/src/libtomcrypt/src/headers/tomcrypt_cfg.h \
remote/src/libtomcrypt/src/headers/tomcrypt_cipher.h \
remote/src/libtomcrypt/src/headers/tomcrypt_custom.h \
remote/src/libtomcrypt/src/headers/tomcrypt_hash.h \
remote/src/libtomcrypt/src/headers/tomcrypt_mac.h \
remote/src/libtomcrypt/src/headers/tomcrypt_macros.h \
remote/src/libtomcrypt/src/headers/tomcrypt_math.h \
remote/src/libtomcrypt/src/headers/tomcrypt_misc.h \
remote/src/libtomcrypt/src/headers/tomcrypt_pk.h \
remote/src/libtomcrypt/src/headers/tomcrypt_pkcs.h \
remote/src/libtomcrypt/src/headers/tomcrypt_prng.h \
remote/src/zlib/crc32.h \
remote/src/zlib/gzguts.h \
remote/src/zlib/inffast.h \
remote/src/zlib/inffixed.h \
remote/src/zlib/inflate.h \
remote/src/zlib/inftrees.h \
remote/src/zlib/zconf.h \
remote/src/zlib/zlib.h \
remote/src/zlib/zutil.h

SOURCES += remote/src/jenkins/lookup3.c \
remote/src/libtomcrypt/src/hashes/hash_memory.c \
remote/src/libtomcrypt/src/hashes/md5.c \
remote/src/libtomcrypt/src/misc/crypt_argchk.c \
remote/src/libtomcrypt/src/misc/crypt_hash_descriptor.c \
remote/src/libtomcrypt/src/misc/crypt_hash_is_valid.c \
remote/src/libtomcrypt/src/misc/crypt_libc.c \
remote/src/zlib/adler32.c \
remote/src/zlib/crc32.c \
remote/src/zlib/inffast.c \
remote/src/zlib/inflate.c \
remote/src/zlib/inftrees.c \
remote/src/zlib/zutil.c \
remote/src/CascBuildCfg.cpp \
remote/src/CascCommon.cpp \
remote/src/CascDecompress.cpp \
remote/src/CascFindFile.cpp \
remote/src/CascMndxRoot.cpp \
remote/src/CascOpenFile.cpp \
remote/src/CascOpenStorage.cpp \
remote/src/CascReadFile.cpp \
remote/src/common/Common.cpp \
remote/src/common/Directory.cpp \
remote/src/common/FileStream.cpp \
remote/src/common/ListFile.cpp \
remote/src/common/Map.cpp
