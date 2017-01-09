sudo apt-get install -y -qq autoconf2.13 && \
export SHELL=/bin/bash && \
export PATH=/usr/lib/ccache:$PATH && \
mkdir -p ${WORKSPACE}/.ccache && export CCACHE_DIR=${WORKSPACE}/.ccache && \
mkdir -p ${WORKSPACE}/.npm && export npm_config_cache=${WORKSPACE}/.npm && \
source /etc/profile.d/rvm.sh && \
sudo couchdb -b && \
cd build && \
make -j4 && \
make -j4 test && \
sudo couchdb -d
