TEMPLATE = lib
CONFIG += staticlib

TARGET = casc

HEADERS += ./src/CascCommon.h \
./src/CascLib.h \
./src/CascMndxRoot.h \
./src/CascPort.h \
./src/common/Common.h \
./src/common/FileStream.h \
./src/common/ListFile.h \
./src/common/Map.h \
./src/jenkins/lookup.h \
./src/libtomcrypt/src/headers/tomcrypt.h \
./src/libtomcrypt/src/headers/tomcrypt_argchk.h \
./src/libtomcrypt/src/headers/tomcrypt_cfg.h \
./src/libtomcrypt/src/headers/tomcrypt_cipher.h \
./src/libtomcrypt/src/headers/tomcrypt_custom.h \
./src/libtomcrypt/src/headers/tomcrypt_hash.h \
./src/libtomcrypt/src/headers/tomcrypt_mac.h \
./src/libtomcrypt/src/headers/tomcrypt_macros.h \
./src/libtomcrypt/src/headers/tomcrypt_math.h \
./src/libtomcrypt/src/headers/tomcrypt_misc.h \
./src/libtomcrypt/src/headers/tomcrypt_pk.h \
./src/libtomcrypt/src/headers/tomcrypt_pkcs.h \
./src/libtomcrypt/src/headers/tomcrypt_prng.h \
./src/zlib/crc32.h \
./src/zlib/gzguts.h \
./src/zlib/inffast.h \
./src/zlib/inffixed.h \
./src/zlib/inflate.h \
./src/zlib/inftrees.h \
./src/zlib/zconf.h \
./src/zlib/zlib.h \
./src/zlib/zutil.h

SOURCES += ./src/jenkins/lookup3.c \
./src/libtomcrypt/src/hashes/hash_memory.c \
./src/libtomcrypt/src/hashes/md5.c \
./src/libtomcrypt/src/misc/crypt_argchk.c \
./src/libtomcrypt/src/misc/crypt_hash_descriptor.c \
./src/libtomcrypt/src/misc/crypt_hash_is_valid.c \
./src/libtomcrypt/src/misc/crypt_libc.c \
./src/zlib/adler32.c \
./src/zlib/crc32.c \
./src/zlib/inffast.c \
./src/zlib/inflate.c \
./src/zlib/inftrees.c \
./src/zlib/zutil.c \
./src/CascBuildCfg.cpp \
./src/CascCommon.cpp \
./src/CascDecompress.cpp \
./src/CascFindFile.cpp \
./src/CascMndxRoot.cpp \
./src/CascOpenFile.cpp \
./src/CascOpenStorage.cpp \
./src/CascReadFile.cpp \
./src/common/Common.cpp \
./src/common/Directory.cpp \
./src/common/FileStream.cpp \
./src/common/ListFile.cpp \
./src/common/Map.cpp
