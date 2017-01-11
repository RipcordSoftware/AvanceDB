sudo yum install -y autoconf213 && \
export SHELL=/bin/bash && \
export PATH=/usr/lib64/ccache:$PATH && \
mkdir -p ${WORKSPACE}/.ccache && export CCACHE_DIR=${WORKSPACE}/.ccache && \
mkdir -p ${WORKSPACE}/.npm && export npm_config_cache=${WORKSPACE}/.npm && \
source /etc/profile.d/rvm.sh && \
sudo /usr/local/bin/couchdb -b && \
cd build && \
make -j4 CC=gcc-4.9 CXX=g++-4.9 && \
make -j4 CC=gcc-4.9 CXX=g++-4.9 test && \
sudo /usr/local/bin/couchdb -d
