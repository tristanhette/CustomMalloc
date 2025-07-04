#!/bin/sh
LD_PRELOAD=./libmalloc.so ls
if [ "$DIFF" != "" ]; then
    printf "\e[31m[ FAIL ]\e[39m  ";
    printf "%b\n" "$var"
    printf "%s\n" $DIFF;
  else
    printf "\e[32m[ OK ]\e[39m\t  "
    printf "%b\n" "$var"
fi

LD_PRELOAD=./libmalloc.so find
if [ "$DIFF" != "" ]; then
    printf "\e[31m[ FAIL ]\e[39m  ";
    printf "%b\n" "$var"
    printf "%s\n" $DIFF;
  else
    printf "\e[32m[ OK ]\e[39m\t  "
    printf "%b\n" "$var"
fi

LD_PRELOAD=./libmalloc.so cat Makefile
if [ "$DIFF" != "" ]; then
    printf "\e[31m[ FAIL ]\e[39m  ";
    printf "%b\n" "$var"
    printf "%s\n" $DIFF;
  else
    printf "\e[32m[ OK ]\e[39m\t  "
    printf "%b\n" "$var"
fi

LD_PRELOAD=./libmalloc.so ip a
if [ "$DIFF" != "" ]; then
    printf "\e[31m[ FAIL ]\e[39m  ";
    printf "%b\n" "$var"
    printf "%s\n" $DIFF;
  else
    printf "\e[32m[ OK ]\e[39m\t  "
    printf "%b\n" "$var"
fi
LD_PRELOAD=./libmalloc.so factor 20 30 40 50 60 70 80 90
if [ "$DIFF" != "" ]; then
    printf "\e[31m[ FAIL ]\e[39m  ";
    printf "%b\n" "$var"
    printf "%s\n" $DIFF;
  else
    printf "\e[32m[ OK ]\e[39m\t  "
    printf "%b\n" "$var"
fi
LD_PRELOAD=./libmalloc.so git status
if [ "$DIFF" != "" ]; then
    printf "\e[31m[ FAIL ]\e[39m  ";
    printf "%b\n" "$var"
    printf "%s\n" $DIFF;
  else
    printf "\e[32m[ OK ]\e[39m\t  "
    printf "%b\n" "$var"
fi
LD_PRELOAD=./libmalloc.so clang --help
if [ "$DIFF" != "" ]; then
    printf "\e[31m[ FAIL ]\e[39m  ";
    printf "%b\n" "$var"
    printf "%s\n" $DIFF;
  else
    printf "\e[32m[ OK ]\e[39m\t  "
    printf "%b\n" "$var"
fi
