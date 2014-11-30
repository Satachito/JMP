#pragma once
//	https://github.com/Satachito/JpMooParaiso.git
//	Written by Satoru Ogura Tokyo, Japan.

#include	<cassert>

#if TARGET_OS_IPHONE
#include	<OpenGLES/ES2/gl.h>
#import		<GLKit/GLKit.h>
#else
#include	<OpenGL/gl.h>
#error Todavia no he aprovado en OSX.
#endif


namespace
JpMooParaiso
{	namespace
	OpenGL
	{
		struct
		OpenGLuint
		{	GLuint	u;
			OpenGLuint( GLuint p )
			: u( p )
			{
			}
			operator
			GLuint()
			{	return u;
			}
		};
/*
		inline	NSString*
		ShaderSource( NSString* pType )
		{	return
			[	NSString
				stringWithContentsOfFile:[ NSBundle.mainBundle pathForResource:@"Shader" ofType:pType ]
				encoding:NSUTF8StringEncoding
				error:nil
			];
		}
*/
		inline	GLuint
		CreateShader( const GLchar* pSource, GLenum pType )
		{	GLuint v = glCreateShader( pType );
			glShaderSource( v, 1, &pSource, nullptr );
			glCompileShader( v );
			GLint wLogLength;
			glGetShaderiv( v, GL_INFO_LOG_LENGTH, &wLogLength );
			if ( wLogLength )
			{	GLchar wLog[ wLogLength ];
				glGetShaderInfoLog( v, wLogLength, &wLogLength, wLog );
				printf( "Shader compile source:\n%s\nlog:\n%s\n", pSource, wLog);
			}
			GLint wStatus;
			glGetShaderiv( v, GL_COMPILE_STATUS, &wStatus);
			assert( wStatus );
			return v;
		}

		struct
		VertShader : OpenGLuint
		{	
		~	VertShader()
			{	glDeleteShader( u );
			}
			
			VertShader( const GLchar* p )
			: OpenGLuint( CreateShader( p, GL_VERTEX_SHADER ) )
			{
			}
		};

		struct
		FragShader : OpenGLuint
		{	
		~	FragShader()
			{	glDeleteShader( u );
			}
			
			FragShader( const GLchar* p )
			: OpenGLuint( CreateShader( p, GL_FRAGMENT_SHADER ) )
			{
			}
		};

		struct
		Matrix
		{	GLfloat	u[ 16 ];
			operator
			GLfloat*()
			{	return u;
			}
		};

		inline	Matrix
		OrthogonalMatrix
		(	float pLeft
		,	float pRight
		,	float pBottom
		,	float pTop
		,	float pNear
		,	float pFar
		)
		{
			Matrix	v;
			float wDX = pRight - pLeft;
			float wDY = pTop - pBottom;
			float wDZ = pFar - pNear;

			v.u[ 0] =  2.0f / wDX;
			v.u[ 5] =  2.0f / wDY;
			v.u[10] = -2.0f / wDZ;
			v.u[12] = -(pRight + pLeft) / wDX;
			v.u[13] = -(pTop + pBottom) / wDY;
			v.u[14] = -(pFar + pNear) / wDZ;
			v.u[15] =  1.0f;
			v.u[ 1] = v.u[ 2] = v.u[ 3] = v.u[ 4] =
			v.u[ 6] = v.u[ 7] = v.u[ 8] = v.u[ 9] = v.u[11] = 0.0f;

			return v;
		}

		inline	Matrix
		PerspectiveMatrix
		(	float pLeft
		,	float pRight
		,	float pBottom
		,	float pTop
		,	float pNear
		,	float pFar
		)
		{
			Matrix	v;
			float wDX = pRight - pLeft;
			float wDY = pTop - pBottom;
			float wDZ = pFar - pNear;

			v.u[ 0] =  2.0f * pNear / wDX;
			v.u[ 5] =  2.0f * pNear / wDY;
			v.u[ 8] =  (pRight + pLeft) / wDX;
			v.u[ 9] =  (pTop + pBottom) / wDY;
			v.u[10] = -(pFar + pNear) / wDZ;
			v.u[11] = -1.0f;
			v.u[14] = -2.0f * pFar * pNear / wDZ;
			v.u[ 1] = v.u[ 2] = v.u[ 3] = v.u[ 4] =
			v.u[ 6] = v.u[ 7] = v.u[12] = v.u[13] = v.u[15] = 0.0f;
			
			return v;
		}

		struct
		Program : OpenGLuint
		{
			GLint	uTransformUniform;
			GLint	uNormalUniform;
			
		~	Program()
			{	glDeleteProgram( u );
			}
			Program( const GLchar* pVertShaderSource, const GLchar* pFragShaderSource )
			: OpenGLuint( glCreateProgram() )
			{
				glBindAttribLocation( u, GLKVertexAttribPosition, "position" );
				glBindAttribLocation( u, GLKVertexAttribNormal, "normal" );
//				glBindAttribLocation( u, GLKVertexAttribColor, "color" );
//				glBindAttribLocation( u, GLKVertexAttribTexCoord0, "texCoord0" );
//				glBindAttribLocation( u, GLKVertexAttribTexCoord1, "texCoord1" );

				{	VertShader	wVertShader( pVertShaderSource );
					FragShader	wFragShader( pFragShaderSource );

					glAttachShader( u, wVertShader );
					glAttachShader( u, wFragShader );

					glLinkProgram( u );
					GLint wLogLength;
					glGetProgramiv( u, GL_INFO_LOG_LENGTH, &wLogLength );
					if ( wLogLength )
					{	GLchar wLog[ wLogLength ];
						glGetProgramInfoLog( u, wLogLength, &wLogLength, wLog );
						printf( "Program link log:\n%s\n", wLog );
					}
					GLint wStatus;
					glGetProgramiv( u, GL_LINK_STATUS, &wStatus );
					assert( wStatus );

					glDetachShader( u, wVertShader );
					glDetachShader( u, wFragShader );
				}

				uTransformUniform = glGetUniformLocation( u, "Transform" );
				uNormalUniform = glGetUniformLocation( u, "Normal" );
			}

			void
			Prepare( GLKMatrix3 pNormal, GLKMatrix4 pProjection, GLKMatrix4 pModelView )
			{	glUseProgram( u );
				glUniformMatrix4fv
				(	uTransformUniform
				,	1
				,	0
				,	GLKMatrix4Multiply( pProjection, pModelView ).m
				);
				glUniformMatrix3fv
				(	uNormalUniform
				,	1
				,	0
				,	pNormal.m
				);
			}
		};
	}
}


