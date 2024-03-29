// credit: https://googlechrome.github.io/samples/service-worker/basic/

const version = 13
const PRECACHE = 'precache-v' + version;
const RUNTIME = 'runtime-v' + version;

const PRECACHE_URLS = [
  '/MonCraft.data',
  '/MonCraft.wasm',
  '/MonCraft.js',
  '/MonCraft.worker.js',
];

// The install handler takes care of precaching the resources we always need.
self.addEventListener('install', event => {
  caches.delete(PRECACHE);
  caches.delete(RUNTIME);
  event.waitUntil(
    caches.open(PRECACHE)
      .then(cache => cache.addAll(PRECACHE_URLS))
      .then(self.skipWaiting())
  );
});

// The activate handler takes care of cleaning up old caches.
self.addEventListener('activate', event => {
  event.waitUntil(
    caches.keys().then(cacheNames => {
      return cacheNames.filter(cacheName => cacheName !== PRECACHE);
    }).then(cachesToDelete => {
      return Promise.all(cachesToDelete.map(cacheToDelete => {
        return caches.delete(cacheToDelete);
      }));
    }).then(() => self.clients.claim())
  );
});

// The fetch handler serves responses for same-origin resources from a cache.
// If no response is found, it populates the runtime cache with the response
// from the network before returning it to the page.
self.addEventListener('fetch', event => {
  // Skip cross-origin requests, like those for Google Analytics.
  if (event.request.url.startsWith(self.location.origin)) {
    event.respondWith(
      caches.match(event.request).then(cachedResponse => {
        if (cachedResponse) {
          console.log('using cached', event.request.url)
          return cachedResponse;
        }
        else {
          console.log('downloading', event.request.url)
          return caches.open(RUNTIME).then(cache => {
            return fetch(event.request).then(response => {
              // Put a copy of the response in the runtime cache.
              return cache.put(event.request, response.clone()).then(() => {
                return response;
              });
            });
          });
        }
      })
    );
  }
});