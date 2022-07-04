DIR=`cd $(dirname $0); pwd -P`
cd $DIR
cargo build
install_name_tool -add_rpath ${DIR} target/debug/rustdemo