
#include "VK_Window.h"

/*
	   _---~~(~~-_.
	 _{        )   )
   ,   ) -~~- ( ,-' )_
  (  `-,_..`., )-- '_,)
 ( ` _)  (  -~( -_ `,  }
 (_-  _  ~_-~~~~`,  ,' )
   `~ -^(    __;-,((()))
		 ~~~~ {_ -_(())
				`\  }
				  { }
*/

int main() {

	VK_Window vk_Window;

	try {
		vk_Window.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	

	return EXIT_SUCCESS;
}