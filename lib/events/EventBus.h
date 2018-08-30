/*
 * EventBus.h, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */

#pragma once

#include <vcmi/events/SubscriptionRegistry.h>

namespace events
{

class EventBus
{
public:
	template <typename E>
	std::unique_ptr<EventSubscription> subscribeBefore(typename E::PreHandler && cb)
	{
		auto registry = SubscriptionRegistry<E>::get();
		return registry->subscribeBefore(this, std::move(cb));
	}

	template <typename E>
	std::unique_ptr<EventSubscription> subscribeAfter(typename E::PostHandler && cb)
	{
		auto registry = SubscriptionRegistry<E>::get();
		return registry->subscribeAfter(this, std::move(cb));
	}

	template <typename E>
	void executeEvent(E & event) const
	{
		SubscriptionRegistry<E>::get()->executeEvent(this, event);
	}
};
}
