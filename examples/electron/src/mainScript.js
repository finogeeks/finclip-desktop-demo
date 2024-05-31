window.openFinClipWindow = () => {
  const domain = document.getElementById('domain').value;
  const appkey = document.getElementById('appkey').value;
  const appid = document.getElementById('appid').value;
  const secret = document.getElementById('secret').value;
  finclip.open({
    domain, appkey, appid, secret,
  });
};

window.openEmbedFinClipWindow = () => {
  const domain = document.getElementById('domain').value;
  const appkey = document.getElementById('appkey').value;
  const appid = document.getElementById('appid').value;
  const secret = document.getElementById('secret').value;
  finclip.embed({
    domain, appkey, appid, secret,
  });
};

window.closeFinClipWindow = () => {
  finclip.close();
};
