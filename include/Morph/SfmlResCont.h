#ifndef _MORPH_SFML_RES_CONT_H_
#define _MORPH_SFML_RES_CONT_H_

#include <unordered_map>

namespace morph {
	template <class T> class SfmlResContainer {
		typedef std::unordered_map<std::string, T*> SfmlResMap;

		SfmlResMap container;

	public:
		const T& getSfmlRes(std::string path);
		bool putSfmlRes(std::string path);
		void releaseAll();

		~SfmlResContainer() { releaseAll(); }

		struct ErrNotFound {
			std::string path;

			ErrNotFound(const std::string &Path) : path(Path) {}
		};
	};

	template <class T> const T& SfmlResContainer<T>::getSfmlRes(std::string path) {
		try {
			return *container.at(path);
		} catch (const std::out_of_range&) {
			std::pair<std::string, T*> elem;
			
			elem.first = path;
			elem.second = new T();
			if (!elem.second->loadFromFile(path)) {
				delete elem.second;
	
				SfmlResContainer::ErrNotFound e(path);
				throw e;
			}
	
			std::pair<SfmlResMap::const_iterator, bool> loc = container.insert(elem);
			if (!loc.second) {
				delete elem.second;
	
				SfmlResContainer::ErrNotFound e(path);
				throw e;
			}
	
			return *loc.first->second;
		}
	}
	
	template <class T> bool SfmlResContainer<T>::putSfmlRes(std::string path) {
		std::pair<std::string, T*> elem;
			
		elem.first = path;
		elem.second = new T();
		if (!elem.second->loadFromFile(path)) {
			return false;
		}
		
		std::pair<SfmlResMap::const_iterator, bool> loc = container.insert(elem);
		return loc.second;
	}
	
	template <class T> void SfmlResContainer<T>::releaseAll() {
		for (SfmlResMap::iterator iter = container.begin(); iter != container.end(); ++iter) {
			delete iter->second;
		}
	
		container.clear();
	}
}

#endif