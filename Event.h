#pragma once
#include <list>
#include <memory>

template<typename F>
class Event;

template<typename R, typename... Args>
class Event<R(Args...)> {
private:
	typedef Event<R(Args...)> ThisEvent;

public:
	class Listener {
		friend class ThisEvent;
	private:
		std::list<ThisEvent *> eventHosts;

		void addEventHost(ThisEvent * theEvent) {
			if (nullptr != theEvent && this->eventHosts.end() == std::find(this->eventHosts.begin(), this->eventHosts.end(), theEvent)) {
				this->eventHosts.push_back(theEvent);
			}
		}

		void removeEventHost(ThisEvent * theEvent) {
			this->eventHosts.remove(theEvent);
		}

		virtual R raise(Args... args) = 0;
	public:
		~Listener() {
			std::list<ThisEvent *> eventHosts = this->eventHosts;
			for (ThisEvent * theEvent : eventHosts) {
				theEvent->removeListener(*this);
			}
		}
	};

	template<typename T>
	class MemberFunctionListener : public Listener {
	private:
		typedef R(T::*CallbackPtr)(Args...);

		T * object = nullptr;
		CallbackPtr callback = nullptr;
	public:
		MemberFunctionListener(T * object, CallbackPtr callback) {
			this->object = object;
			this->callback = callback;
		}

		virtual R raise(Args... args) {
			if (nullptr != this->callback && nullptr != this->object) {
				(this->object->*this->callback)(args...);
			}
		}
	};

	class FreeFunctionListener : public Listener {
	private:
		typedef R(*CallbackPtr)(Args...);
		CallbackPtr callback;

	public:
		FreeFunctionListener(CallbackPtr callback) {
			this->callback = callback;
		}

		virtual R raise(Args... args) {
			if (nullptr != this->callback) {
				(*this->callback)(args...);
			}
		}
	};

private:
	std::list<Listener *> listeners;

public:
	~Event() {
		std::list<Listener *> listeners = this->listeners;
		for (Listener * listener : listeners) {
			listener->removeEventHost(this);
		}
	}

	void addListener(Listener & listener) {
		if (this->listeners.end() == std::find(this->listeners.begin(), this->listeners.end(), &listener)) {
			this->listeners.push_back(&listener);
			listener.addEventHost(this);
		}
	}

	void removeListener(Listener & listener) {
		this->listeners.remove(&listener);
		listener.removeEventHost(this);
	}

	R raise(Args... args) const {
		for (Listener * listener : this->listeners) {
			listener->raise(args...);
		}
	}
};