#!/bin/sh


TOP=`pwd`


echo "�ġ���򥳥�ѥ�����... "

	cd $TOP/src/tool/kp
	make


echo "boot �ץ����κ�����..."

	cd $TOP/src/boot
	make


echo "�濴�� (ITRON) �κ�����..."

	cd $TOP/src/kernel/itron-3.0/make
	make


echo "�����ͥ�饤�֥��(���ճ��ѥ饤�֥��)�κ�����..."

	cd $TOP/src/kernel/kernlib
	make


echo "�ݡ��ȥޥ͡����� (portmanager) �κ�����..."

	cd $TOP/src/kernel/servers
	make


echo "OS ���᡼���κ�����..."

	cd $TOP/src/kernel/make
	make
