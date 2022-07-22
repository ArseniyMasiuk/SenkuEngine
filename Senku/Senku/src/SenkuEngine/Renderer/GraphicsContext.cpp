#include "PrecompiledHeader.h"
#include "SenkuEngine\Core\Base.h"
#include "GraphicsContext.h"
#include "Platform\OpenGL\OpenGLContrext.h"


namespace Senku
{

	Scope<GraphicsContext> GraphicsContext::Create(void * window)
	{
		// todo: add switch case and detect renderer api to create correct graphic context
		// for now it will create only openGLContext
		return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
	}
}
