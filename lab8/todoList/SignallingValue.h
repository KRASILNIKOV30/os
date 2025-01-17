#pragma once
#include <boost/signals2.hpp>

using Connection = boost::signals2::connection;
using ScopedConnection = boost::signals2::scoped_connection;

using SharedConnectionBlock = boost::signals2::shared_connection_block;

template <typename Signature>
using Signal = boost::signals2::signal<Signature>;

using EmptySignal = Signal<void()>;

template <typename T>
class SignallingValue final
{
public:
	using Signal2 = Signal<void(const T& oldValue, const T& newValue)>;
	using Slot2 = typename Signal2::slot_type;
	using Slot1 = typename Signal<void(const T& newValue)>::slot_type;
	using Slot0 = Signal<void()>::slot_type;

	SignallingValue() = default;
	explicit SignallingValue(const T& value)
		: m_value(value)
	{
	}

	T GetValue() const
	{
		return m_value;
	}

	void SetValue(T value)
	{
		if (value != m_value) //-V550
		{
			using std::swap;
			swap(value, m_value);
			// value holds old Value, m_value - new value
			m_changeSignal(value, m_value);
		}
	}

	const T& operator*() const
	{
		return m_value;
	}

	void operator=(const T& newValue)
	{
		SetValue(newValue);
	}

	operator T() const
	{
		return m_value;
	}

	ScopedConnection Connect2(const Slot2& slot, bool sendInitialNotification)
	{
		if (sendInitialNotification)
		{
			slot(m_value, m_value);
		}
		return m_changeSignal.connect(slot);
	}

	ScopedConnection Connect1(const Slot1& slot, bool sendInitialNotification)
	{
		return Connect2([=](const T& /*oldValue*/, const T& newValue) {
			slot(newValue);
		},
			sendInitialNotification);
	}

	ScopedConnection Connect(const Slot0& slot, bool sendInitialNotification)
	{
		return Connect2([=](const T& /*oldValue*/, const T& /*newValue*/) {
			slot();
		},
			sendInitialNotification);
	}

	// No copy/assignment
	SignallingValue(const SignallingValue&) = delete;
	SignallingValue& operator=(const SignallingValue&) = delete;

private:
	T m_value = T();
	Signal2 m_changeSignal;
};