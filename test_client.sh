#! /bin/bash

case "$1" in
"1")
	echo "getting google.com page"
	../web-client http://google.com/index.html
	;;
"2")
	echo "getting one local host object"
	../web-client http://localhost:4000/test/make_test
	;;
"3")
	echo "getting two local host objects"
	../web-client http://localhost:4000/test/make_test http://localhost:4000/test/test.h
	;;
"4")
	echo "getting two local host objects and awechords.com"
	../web-client http://localhost:4000/test/make_test http://localhost:4000/test/test.h http://awechords.com/index.html
	;;
*)
	echo "not supported"
	;;
esac;