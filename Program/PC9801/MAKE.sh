#!/bin/sh


TOP=`pwd`


echo "ツールをコンパイル中... "

	cd $TOP/src/tool/kp
	make


echo "boot プログラムの作成中..."

	cd $TOP/src/boot
	make


echo "中心核 (ITRON) の作成中..."

	cd $TOP/src/kernel/itron-3.0/make
	make


echo "カーネルライブラリ(周辺核用ライブラリ)の作成中..."

	cd $TOP/src/kernel/kernlib
	make


echo "ポートマネージャ (portmanager) の作成中..."

	cd $TOP/src/kernel/servers
	make


echo "OS イメージの作成中..."

	cd $TOP/src/kernel/make
	make
