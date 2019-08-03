FROM alpine:latest
LABEL maintainer="lynx <wyy.hxl@gmail.com>"

ENV LUA_VERSION 5.3.5

RUN set -ex                                     \
    && apk update && apk upgrade                \
    && apk add --no-cache --virtual .build-deps \
        gcc                                     \
        g++                                     \
        make                                    \
        linux-headers                           \
        readline-dev                            \
        musl-dev                                \
    && apk add --no-cache --virtual .tool wget  \
    && cd /tmp;                                 \
        wget -S https://www.lua.org/ftp/lua-${LUA_VERSION}.tar.gz; \
        tar -zxvf lua-${LUA_VERSION}.tar.gz;    \
        cd lua-${LUA_VERSION};                  \
        make linux; make install                \
    && apk del .tool
