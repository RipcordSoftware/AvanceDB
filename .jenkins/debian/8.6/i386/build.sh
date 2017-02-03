sudo apt-get install -y -qq autoconf2.13 && \
export SHELL=/bin/bash && \
export PATH=/usr/lib/ccache:$PATH && \
mkdir -p ${WORKSPACE}/.ccache && export CCACHE_DIR=${WORKSPACE}/.ccache && \
mkdir -p ${WORKSPACE}/.npm && export npm_config_cache=${WORKSPACE}/.npm && \
mkdir -p ${WORKSPACE}/.gem && export GEM_HOME=${WORKSPACE}/.gem && export PATH=$GEM_HOME/bin:$PATH && \
echo '********************************' && ccache -s && echo '********************************' && \
sudo couchdb -b && \
cd build && \
make CFLAGS='-m32' CXXFLAGS='-m32' MOZJS_CONFIG_FLAGS='--host=i386-linux-gnu' -j4 && \
make CFLAGS='-m32' CXXFLAGS='-m32' MOZJS_CONFIG_FLAGS='--host=i386-linux-gnu' -j4 test && \
sudo couchdb -d && \
echo '********************************' && ccache -s && echo '********************************'
