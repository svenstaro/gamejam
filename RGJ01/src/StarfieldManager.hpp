#ifndef STARFIELDMANAGER_HPP_INCLUDED
#define STARFIELDMANAGER_HPP_INCLUDED

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/foreach.hpp>

#include <SFML/Graphics.hpp>

#include <stdint.h>
#include <iostream>

#include "Star.hpp"

class StarfieldManager {
public:
	StarfieldManager();
	~StarfieldManager();
	void SetMaxStars(const uint16_t max_stars);
	const uint16_t GetMaxStars() const;
	const boost::ptr_vector<Star>& GetStarVector() const;
	void Update(float elapsed);

private:
	uint8_t m_current_stars;
	uint8_t m_max_stars;

	boost::ptr_vector<Star> m_stars;
};

#endif
