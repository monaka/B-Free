#
# 不精者のあなたのために。
# この Makefile を使って make をすれば、boot と ITRON ができます。
#
# 実行方法は、この Makefile があるディレクトリで
# 
# 'make <return>' 
#
# を入力するだけです。
#
# すると、
#
# boot/bootimage と kernel/make/btron 
#
# という2つのファイルができます。
#
# この2つのファイルをそれぞれ別の FD にコピーすると、PC9801 で立
# ち上げることができます。
#

all: boot/bootimage kernel/make/btron

boot/bootimage:
	cd boot; make

kernel/make/btron: tool/kp/kp kernel/itron-3.0/make/itron.image kernel/test/echo
	(cd kernel/make; make)

tool/kp/kp:
	(cd tool/kp; make)

kernel/itron-3.0/make/itron.image:
	(cd kernel/itron-3.0/make; make)

kernel/test/echo: kernel/kernlib/libkernel.a
	(cd kernel/test; make)

kernel/kernlib/libkernel.a:
	(cd kernel/kernlib; make)