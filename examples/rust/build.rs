fn main() {
  println!("cargo:rustc-link-lib=dylib=FinClipSDKWrapper");
  println!("cargo:rustc-link-search=native=/path/to/libFinClipSDKWrapper.so");
  // println!("cargo:rustc-link-search=native=.");
}
