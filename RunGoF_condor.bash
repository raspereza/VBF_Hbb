#!/bin/bash
# $1 : workspace w/o root extension
# $2 : algo = (AD, KS, saturated)
ws=$1
algo=$2
dir=/nfs/dust/cms/user/rasp/Run/VBF_Hbb/jobs
workspacedir=${CMSSW_BASE}/src/VBF_Hbb/preapproval_v2
outputdir=${CMSSW_BASE}/src/VBF_Hbb/GoF/

jobname=GoF_${model}_${ws}_${algo}_obs
cat > ${dir}/${jobname}.sh <<EOF1
#!/bin/sh
source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc6_amd64_gcc700
cd ${CMSSW_BASE}/src
cmsenv
cd ${outputdir}
ulimit -s unlimited
combine -M GoodnessOfFit --setParameters r_Hbb=1 -d ${workspacedir}/${ws}.root -m 125 --algo ${algo} -n .${ws}_${algo}.obs
EOF1
chmod u+x ${dir}/jobs/${jobname}.sh
cat > ${dir}/jobs/${jobname}.submit << EOF2
+RequestRuntime=3000

RequestMemory = 2000

executable = ${dir}/jobs/${jobname}.sh

transfer_executable = True
universe            = vanilla
getenv              = True
Requirements        = OpSysAndVer == "CentOS7"

output              = ${dir}/jobs/${jobname}.out
error               = ${dir}/jobs/${jobname}.error
log                 = ${dir}/jobs/${jobname}.log

queue
EOF2
chmod u+x ${dir}/${jobname}.submit
condor_submit ${dir}/${jobname}.submit
#
for i in {1..50}
do
    jobname=GoF_${ws}_${model}_${algo}_${i}
    random=$RANDOM
    cat > ${dir}/${jobname}.sh << EOF3
#!/bin/sh
source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc6_amd64_gcc700
cd ${CMSSW_BASE}/src
cmsenv
cd ${outputdir}
ulimit -s unlimited
combine -M GoodnessOfFit --setParameters r_Hbb=1 -d ${workspacedir}/${ws}.root --toysFreq -m 125 --algo ${algo} -n .${ws}_${algo}_${i}.exp -t 20 -s ${random}
EOF3
    chmod u+x ${dir}/${jobname}.sh
    cat > ${dir}/${jobname}.submit << EOF4
+RequestRuntime=6000

RequestMemory = 2000

executable = ${dir}/${jobname}.sh

transfer_executable = True
universe            = vanilla
getenv              = True
Requirements        = OpSysAndVer == "CentOS7"

output              = ${dir}/${jobname}.out
error               = ${dir}/${jobname}.error
log                 = ${dir}/${jobname}.log

queue
EOF4
    chmod u+x ${dir}/${jobname}.submit
    condor_submit ${dir}/${jobname}.submit
done
