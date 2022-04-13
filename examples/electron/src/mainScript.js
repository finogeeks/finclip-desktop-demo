window.openFinClipWindow = () => {
  finclip.open();
};

window.closeFinClipWindow = () => {
  finclip.close();
};

window.setFinClipWindowSize = () => {
  const left = +document.getElementById('left').value || 0;
  const top = +document.getElementById('top').value || 0;
  const width = +document.getElementById('width').value || 640;
  const height = +document.getElementById('height').value || 480;
  finclip.setSize({ width, height, left, top });
};