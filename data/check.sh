#!/bin/zsh

if [ $# != 1 ] || [ $1 = "-h" ] || [ $1 = "--help" ];then
	echo "usage: ./check.sh program_name"
	exit
fi

pro=$1
tmp=temp

if [ -d "$tmp" ];then
	rm -rf $tmp
fi
mkdir $tmp

echo "\033[1mSubtask 1: Big Integer Support\033[0m"
datapath=BigIntegerTest
state=1
for i in {1..20}; do
	./$pro < $datapath/$i.in > $tmp/$i.out
	t=$?
	if [ $t != 0 ]; then
		echo "	\033[31mProgram exited with code $t on testcase $i\033[0m"
		state=0
		continue
	fi
	diff -w $datapath/$i.out $tmp/$i.out
	t=$?
	if [ $t != 0 ]; then
		echo "	\033[31mWrong Answer on testcase $i\033[0m"
		state=0
		continue
	fi
	echo "	\033[32mtestcase $i Accepted"
done
if [ $state = 1 ]; then
	echo "\033[1m\033[32mSubtask 1 Accepted!\033[0m"
else
	echo "\033[1m\033[31mFailed to pass Subtask 1\033[0m"
fi

echo "\033[1mSubtask 2: Sample\033[0m"
datapath=Sample
state=1
for i in {21..34}; do
	./$pro < $datapath/$i.in > $tmp/$i.out
	t=$?
	if [ $t != 0 ]; then
		echo "	\033[31mProgram exited with code $t on testcase $i\033[0m"
		state=0
		continue
	fi
	diff -w $datapath/$i.out $tmp/$i.out
	t=$?
	if [ $t != 0 ]; then
		echo "	\033[31mWrong Answer on testcase $i\033[0m"
		state=0
		continue
	fi
	echo "	\033[32mtestcase $i Accepted"
done
if [ $state = 1 ]; then
	echo "\033[1m\033[32mSubtask 2 Accepted!\033[0m"
else
	echo "\033[1m\033[31mFailed to pass Subtask 2\033[0m"
fi

echo "\033[1mSubtask 3: Advanced Test\033[0m"
datapath=AdvancedTest
state=1
for i in {35..52}; do
	./$pro < $datapath/$i.in > $tmp/$i.out
	t=$?
	if [ $t != 0 ]; then
		echo "	\033[31mProgram exited with code $t on testcase $i\033[0m"
		state=0
		continue
	fi
	diff -w $datapath/$i.out $tmp/$i.out
	t=$?
	if [ $t != 0 ]; then
		echo "	\033[31mWrong Answer on testcase $i\033[0m"
		state=0
		continue
	fi
	echo "	\033[32mtestcase $i Accepted"
done
if [ $state = 1 ]; then
	echo "\033[1m\033[32mSubtask 3 Accepted!\033[0m"
else
	echo "\033[1m\033[31mFailed to pass Subtask 3\033[0m"
fi

echo "\033[1mSubtask 4: Complex Test\033[0m"
datapath=ComplexTest
state=1
for i in {53..56}; do
	./$pro < $datapath/$i.in > $tmp/$i.out
	t=$?
	if [ $t != 0 ]; then
		echo "	\033[31mProgram exited with code $t on testcase $i\033[0m"
		state=0
		continue
	fi
	diff -w $datapath/$i.out $tmp/$i.out
	t=$?
	if [ $t != 0 ]; then
		echo "	\033[31mWrong Answer on testcase $i\033[0m"
		state=0
		continue
	fi
	echo "	\033[32mtestcase $i Accepted"
done
if [ $state = 1 ]; then
	echo "\033[1m\033[32mSubtask 4 Accepted!\033[0m"
else
	echo "\033[1m\033[31mFailed to pass Subtask 4\033[0m"
fi

echo "\033[1mSubtask 5: Corner Test\033[0m"
datapath=CornerTest
state=1
for i in {57..66}; do
	./$pro < $datapath/$i.in > $tmp/$i.out
	t=$?
	if [ $t != 0 ]; then
		echo "	\033[31mProgram exited with code $t on testcase $i\033[0m"
		state=0
		continue
	fi
	diff -w $datapath/$i.out $tmp/$i.out
	t=$?
	if [ $t != 0 ]; then
		echo "	\033[31mWrong Answer on testcase $i\033[0m"
		state=0
		continue
	fi
	echo "	\033[32mtestcase $i Accepted"
done
if [ $state = 1 ]; then
	echo "\033[1m\033[32mSubtask 5 Accepted!\033[0m"
else
	echo "\033[1m\033[31mFailed to pass Subtask 5\033[0m"
fi

rm -rf $tmp
