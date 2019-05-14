#include "app.hpp"


int main(int argc, char** argv) {
	try {
		App app;
		return app.Start();
	}
	catch (...) {
		return 1;
	}
}


