/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#pragma once

namespace MTP {
namespace internal {

class AbstractSocket : protected QObject {
public:
	static std::unique_ptr<AbstractSocket> Create(
		not_null<QThread*> thread,
		const bytes::vector &secret,
		const ProxyData &proxy);

	explicit AbstractSocket(not_null<QThread*> thread) {
		moveToThread(thread);
	}
	virtual ~AbstractSocket() = default;

	[[nodiscard]] rpl::producer<> connected() const {
		return _connected.events();
	}
	[[nodiscard]] rpl::producer<> disconnected() const {
		return _disconnected.events();
	}
	[[nodiscard]] rpl::producer<> readyRead() const {
		return _readyRead.events();
	}
	[[nodiscard]] rpl::producer<> error() const {
		return _error.events();
	}

	virtual void connectToHost(const QString &address, int port) = 0;
	[[nodiscard]] virtual bool isConnected() = 0;
	[[nodiscard]] virtual int bytesAvailable() = 0;
	[[nodiscard]] virtual int64 read(char *buffer, int64 maxLength) = 0;
	virtual int64 write(const char *buffer, int64 length) = 0;

	virtual int32 debugState() = 0;

protected:
	rpl::event_stream<> _connected;
	rpl::event_stream<> _disconnected;
	rpl::event_stream<> _readyRead;
	rpl::event_stream<> _error;

};

} // namespace internal
} // namespace MTP