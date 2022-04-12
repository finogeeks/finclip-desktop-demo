window.onbeforeunload = (e) => {
  finclip.close();
  e.returnValue = false;
};