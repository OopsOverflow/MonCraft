const statusElement = document.getElementById('status');
const progressElement = document.getElementById('progress');
const moncraftInfo = document.getElementById('moncraft-info');
const moreInfo = document.getElementById('more-info');
const moreInfoBtn = document.getElementById('more-info-btn');
const infoCloseBtn = document.getElementById('info-close');
const canvas = document.getElementById('canvas');

infoCloseBtn.addEventListener('click', () => {
  moncraftInfo.classList.add('hidden');
});

canvas.addEventListener("webglcontextlost", function(e) {
  alert('WebGL context lost. You will need to reload the page.');
  e.preventDefault();
});

function showMoncraftInfo() {
  moncraftInfo.classList.remove('hidden');
}

moreInfoBtn.addEventListener('click', function () {
  moreInfo.classList.toggle('hidden');
  moreInfoBtn.innerHTML = moreInfo.classList.contains('hidden') ? "▾ Plus d'informations ▾" : "▴ Moins d'informations ▴"
});

function updateStatus(text) {
  if (!Module.setStatus.last) Module.setStatus.last = { time: Date.now(), text: '' };
  if (text === Module.setStatus.last.text) return;
  var m = text.match(/([^(]+)\((\d+(\.\d+)?)\/(\d+)\)/);
  var now = Date.now();
  if (m && now - Module.setStatus.last.time < 30) return; // if this is a progress update, skip it if too soon
  Module.setStatus.last.time = now;
  Module.setStatus.last.text = text;
  

  if (m) {
    var mb = (m[2] / 1E6).toPrecision(2)
    var mbMax = (m[4] / 1E6).toPrecision(2)

    text = `${m[1]}<br/>(${mb} / ${mbMax} MB)`;
    progressElement.value = parseInt(m[2])*100;
    progressElement.max = parseInt(m[4])*100;
    progressElement.hidden = false;
  } else {
    progressElement.value = null;
    progressElement.max = null;
    progressElement.hidden = true;
    if (text === "") showMoncraftInfo();
  }

  statusElement.innerHTML = text;
}

async function registerSW() {
  if ('serviceWorker' in navigator) {
    try {
      const reg = await navigator.serviceWorker.register('sw.js', { scope: '/' });
      if(reg.installing) {
        console.log('[SW] installing');
      } else if(reg.waiting) {
        console.log('[SW] installed');
      } else if(reg.active) {
        console.log('[SW] active');
      }
    } catch (e) {
      console.warn('[SW] failed :', error);
    }
  }
  else {
    console.warn('[SW] unavailable');
  }
}

registerSW();

var Module = {
  preRun: [],
  postRun: [],
  canvas,

  print: function(text) { console.log(text); },
  printErr: function(text) { console.error(text); },

  totalDependencies: 0,
  monitorRunDependencies: function(left) {
    this.totalDependencies = Math.max(this.totalDependencies, left);
    Module.setStatus(left ? 'Preparing... (' + (this.totalDependencies-left) + '/' + this.totalDependencies + ')' : 'All downloads complete.');
  },

  setStatus: updateStatus,
};

Module.setStatus('Downloading...');

window.onerror = function(event) {
  // TODO: do not warn on ok events like simulating an infinite loop or exitStatus
  Module.setStatus('Exception thrown, see JavaScript console');
  Module.setStatus = function(text) {
    if (text) Module.printErr('[post-exception status] ' + text);
  };
};
