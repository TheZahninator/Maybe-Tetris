#pragma once
template<typename T>
class BagContainer
{
public:
	BagContainer(){}

	~BagContainer(){}

	void clear(){
		m_content.clear();
	}

	void add(T toAdd){
		m_content.push_back(toAdd);
	}

	T draw(){
		int i = rand() % m_content.size();
		T result = m_content[i];
		m_content.erase(m_content.begin() + i);

		return result;
	}

	bool isEmpty(){
		return m_content.size() == 0;
	}

private:
	std::vector<T> m_content;
};

