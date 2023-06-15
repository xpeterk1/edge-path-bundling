#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/ObjLoader.h"
#include "cinder/app/AppBase.h"
using namespace ci;
using namespace ci::app;

class BasicApp : public App {
public:
	void	setup() override;
	void	draw() override;
	void update() override;

	CameraPersp			mCam;
	gl::BatchRef		mSphere;
	gl::TextureRef		mTexture;
	gl::GlslProgRef		mGlsl;

	float mRotationAngle = 0.0f;
};

void BasicApp::setup()
{
	mCam.lookAt(vec3(3, 2.5, 4), vec3(0));
	auto img = loadImage("spherical_texture.png");
	mTexture = gl::Texture::create(img);
	mTexture->bind();

	auto shader = gl::ShaderDef().texture().lambert();
	mGlsl = gl::getStockShader(shader);
	auto sphere = geom::Sphere().subdivisions(50);
	mSphere = gl::Batch::create(sphere, mGlsl);

	gl::enableDepthWrite();
	gl::enableDepthRead();
	setWindowSize(1024, 768);
}

void BasicApp::update()
{
	// Rotate the sphere
	mRotationAngle += 0.003f;
}

void BasicApp::draw()
{
	gl::clear(Color(0.0f, 0.0f, 0.0f));
	gl::setMatrices(mCam);
	gl::rotate(glm::rotate(mRotationAngle, vec3(0.0f, 1.0f, 0.0f)));
	mSphere->draw();
}

CINDER_APP(BasicApp, RendererGl)

