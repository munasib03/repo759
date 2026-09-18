#!/usr/bin/env bash
#SBATCH -p instruction
#SBATCH -J FirstSlurm
#SBATCH -o FirstSlurm.out -e FirstSlurm.err
#SBATCH -t 0-00:05:00
#SBATCH -c 2

hostname
