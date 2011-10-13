#include <iostream>
#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

using boost::property_tree::ptree;

class Entity {
public:
	Entity(std::string uid, float x=0, float y=0, std::string img="", float rot=0, float a=0, int l=0) {
		muid = uid;
		mx = x;
		my = y;
		mimg = img;
		mrot = rot;
		ma = a;
		ml = l;
	};
	~Entity() {};

	void save(ptree* pt) {
		pt->add("entities."+muid+".x", mx);
		pt->add("entities."+muid+".y", my);
		pt->add("entities."+muid+".img", mimg);
		pt->add("entities."+muid+".rot", mrot);
		pt->add("entities."+muid+".a", ma);
		pt->add("entities."+muid+".l", ml);
	};

	void load(ptree* pt) {
		mx = pt->get<float>("entities."+muid+".x");
		my = pt->get<float>("entities."+muid+".y");
		mimg = pt->get<std::string>("entities."+muid+".img");
		mrot = pt->get<float>("entities."+muid+".rot");
		ma = pt->get<float>("entities."+muid+".a");
		ml = pt->get<int>("entities."+muid+".l");
	};

	void print() {
		std::cout << std::endl;
		std::cout << "Printing entity " << muid << std::endl;
		std::cout << "  x: " << mx << std::endl;
		std::cout << "  y: " << my << std::endl;
		std::cout << "  img: " << mimg << std::endl;
		std::cout << "  rot: " << mrot << std::endl;
		std::cout << "  a: " << ma << std::endl;
		std::cout << "  l: " << ml << std::endl;
	};
private:
	std::string muid;
	float mx;
	float my;
	std::string mimg;
	float mrot;
	float ma; // alpha
	int ml; // layer
};

boost::ptr_vector<Entity> Entities;

bool finished = false;

int main(void) {
	std::cout << "Loading Entities..." << std::endl;
	ptree pt;
	read_info("entities.info", pt);
	BOOST_FOREACH(ptree::value_type &v, pt.get_child("entities")) {
		std::string uid_ = v.first.data();
		Entities.push_back(new Entity(uid_));
		Entities.back().load(&pt);
	}
	while(!finished) {
		std::cout << "Hello, you are in the main menu, choose 1 option:" << std::endl;
		std::cout << "  (1) create new entity" << std::endl;
		std::cout << "  (2) print all entities" << std::endl;
		std::cout << "  (3) save entities" << std::endl;
		std::cout << ">> ";
		std::string input;
		std::cin >> input;
		if (input == "1") {
			std::cout << "Creating new entity..." << std::endl;

			std::cout << "uid: ";
			std::string uid;
			std::cin >> uid;
			std::cout << std::endl;

			std::cout << "x: ";
			std::string x;
			std::cin >> x;
			std::cout << std::endl;

			std::cout << "y: ";
			std::string y;
			std::cin >> y;
			std::cout << std::endl;

			std::cout << "img: ";
			std::string img;
			std::cin >> img;
			std::cout << std::endl;

			std::cout << "rot: ";
			std::string rot;
			std::cin >> rot;
			std::cout << std::endl;

			std::cout << "a: ";
			std::string a;
			std::cin >> a;
			std::cout << std::endl;

			std::cout << "l: ";
			std::string l;
			std::cin >> l;
			std::cout << std::endl;

			Entities.push_back(new Entity(uid, boost::lexical_cast<float>(x),boost::lexical_cast<float>(y),img, boost::lexical_cast<float>(rot),boost::lexical_cast<float>(a),boost::lexical_cast<int>(l)));
			std::cout << "Entitiy created successfully." << std::endl;
		} else if (input == "2") {
			if (Entities.size() > 0) {
				BOOST_FOREACH(Entity& entity, Entities) {
					entity.print();
					std::cout << std::endl;
				}
			} else {
				std::cout << "There are no entities :(" << std::endl;
			}
		} else if (input == "3") {
			if (Entities.size() > 0) {
				ptree pt;
				BOOST_FOREACH (Entity& entity, Entities) {
					entity.save(&pt);
				}
				write_info("entities.info", pt);
			} else {
				std::cout << "There are no entities :(" << std::endl;
			}
		}
	}
}
