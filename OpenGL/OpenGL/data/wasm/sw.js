const CACHE = 'v1';
const WHITELIST = [
  'MonCraft.data'
]

const onInstall = async (e) => {
  const cache = await caches.open(CACHE);
  return cache.addAll(WHITELIST);
};

const onFetch = async (e) => {
  const cache = await caches.open(CACHE);
  let resp = await cache.match(e.request);

  if (resp === undefined) {
    resp = await fetch(e.request);
    for (const str of WHITELIST) {
      if(e.request.url.endsWith(str)) {
        cache.put(e.request, resp.clone());
        break;
      }
    }
  }

  return resp;
}

self.addEventListener('install', function(e) {
  e.waitUntil(onInstall(e));
});

self.addEventListener('fetch', function(e) {
  e.respondWith(onFetch(e));
});
