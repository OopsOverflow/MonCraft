/**
 * Copyright (c) 2017-2022 Paul-Louis Ageneau
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "websocket.hpp"

#include <emscripten/emscripten.h>

#include <exception>
#include <memory>
#include <iostream>

extern "C" {
extern int wsCreateWebSocket(const char *url);
extern void wsDeleteWebSocket(int ws);
extern void wsSetOpenCallback(int ws, void (*openCallback)(void *));
extern void wsSetErrorCallback(int ws, void (*errorCallback)(const char *, void *));
extern void wsSetMessageCallback(int ws, void (*messageCallback)(const char *, int, void *));
extern int EMSCRIPTEN_KEEPALIVE wsSendMessage(int ws, const char *buffer, int size);
extern void wsSetUserPointer(int ws, void *ptr);
}

namespace rtc {

void WebSocket::OpenCallback(void *ptr) {
	WebSocket *w = static_cast<WebSocket *>(ptr);
	if (w)
		w->triggerOpen();
}

void WebSocket::ErrorCallback(const char *error, void *ptr) {
	WebSocket *w = static_cast<WebSocket *>(ptr);
	if (w)
		w->triggerError(string(error ? error : "unknown"));
}

void WebSocket::MessageCallback(const char *data, int size, void *ptr) {
	WebSocket *w = static_cast<WebSocket *>(ptr);
	if (w) {
		if (data) {
			if (size >= 0) {
				auto b = reinterpret_cast<const byte *>(data);
				w->triggerMessage(binary(b, b + size));
			} else {
				w->triggerMessage(string(data));
			}
		} else {
			w->close();
			w->triggerClosed();
		}
	}
}

WebSocket::WebSocket() : mId(0), mConnected(false) {}

WebSocket::~WebSocket() { close(); }

void WebSocket::open(const string &url) {
	close();

	mId = wsCreateWebSocket(url.c_str());
	if (!mId)
		throw std::runtime_error("WebSocket not supported");

	wsSetUserPointer(mId, this);
	wsSetOpenCallback(mId, OpenCallback);
	wsSetErrorCallback(mId, ErrorCallback);
	wsSetMessageCallback(mId, MessageCallback);
}

void WebSocket::close() {
	mConnected = false;
	if (mId) {
		wsDeleteWebSocket(mId);
		mId = 0;
	}
}

bool WebSocket::isOpen() const { return mConnected; }

bool WebSocket::isClosed() const { return mId == 0; }

bool WebSocket::send(message_variant message) {
	// MAIN_THREAD_ASYNC_EM_ASM(console.log('hello'));
	
	if (!mId)
		return false;

	return std::visit(overloaded{
		[this](const binary &b) {
			// for(auto& byte: b)
			// 	std::cout << std::to_integer<unsigned char>(byte) << " ";
			// std::cout << std::endl;
			auto data = reinterpret_cast<const char *>(b.data());
			MAIN_THREAD_EM_ASM({ _wsSendMessage($0, $1, $2) }, mId, data, int(b.size()));
			// emscripten_sync_run_in_main_thread_3((int)wsSendMessage, (void*)mId, (void*)data, (void*)int(b.size()));
			// return wsSendMessage(mId, data, int(b.size())) >= 0;
			return true;
		},
		[this](const string &s) {
			MAIN_THREAD_EM_ASM({ _wsSendMessage($0, $1, $2)}, mId, s.c_str(), -1);
			// emscripten_sync_run_in_main_thread_3((int)wsSendMessage, (void*)mId, (void*)s.c_str(), (void*)-1);
			// return wsSendMessage(mId, s.c_str(), -1) >= 0;
			return true;
		}
	}, std::move(message));
	return true;
}

bool WebSocket::send(const byte *data, size_t size) {
	if (!mId)
		return false;

	return wsSendMessage(mId, reinterpret_cast<const char *>(data), int(size)) >= 0;
}

void WebSocket::triggerOpen() {
	mConnected = true;
	Channel::triggerOpen();
}

} // namespace rtc
