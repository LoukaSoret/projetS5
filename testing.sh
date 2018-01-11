#!/bin/bash

#Auteur: Louka Soret
#Date: 11/01/2018

if [ $# -ne 1 ]
then
	echo "[Erreur] aucun argument."
else
	filename=$1
	baseFilename=$(basename $filename)
	# Recupere le numero de la derniere ligne : swi 0x123456
	lastLine=$(grep -n 'swi 0x123456' 'Examples/'$baseFilename'.s' | awk -F ":" '{print $1}')	
	if [ -f "arm_simulator" ]
	then
		> log_sim.out
		./arm_simulator --gdb-port 44000 &
		arm-none-eabi-gdb -ex "set pagination off" \
		-ex "file $1" \
		-ex "target remote localhost:44000" \
		-ex "load" \
		-ex "b $lastLine" \
		-ex "continue" \
		-ex "set logging file log_sim.out" \
		-ex "set logging on" \
		-ex "inf reg" \
		-ex "set logging off" \
		-ex "continue" \
		-ex "quit" &> /dev/null

		> log_gdb.out
		arm-none-eabi-gdb -ex "set pagination off" \
		-ex "file $1" \
		-ex "target sim" \
		-ex "load" \
		-ex "b 1" \
		-ex "run" \
		-ex "advance $lastLine" \
		-ex "set logging file log_gdb.out" \
		-ex "set logging on" \
		-ex "inf reg" \
		-ex "set logging off" \
		-ex "continue" \
		-ex "quit" &> /dev/null

		echo "--------------------{RESULT}--------------------"
		echo "$(diff log_sim.out log_gdb.out)"
		echo "------------------------------------------------"

	else
		echo "[Erreur] l'executable du simulateur n'est pas present dans le dossier courant."
	fi
fi

