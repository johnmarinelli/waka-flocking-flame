#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

#include "Particle.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

class WakaFlockingFlameApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
  
  CameraPersp mCamera;
  
  std::vector<Particle> mParticles;
  
  std::vector<glm::vec3> getParticlePositions() const;
  std::vector<Particle> getNearbyParticles(const Particle& p) const;
};

std::vector<glm::vec3> WakaFlockingFlameApp::getParticlePositions() const {
  std::vector<glm::vec3> positions;
  
  std::for_each(mParticles.begin(), mParticles.end(), [&positions] (const Particle& p) {
    positions.push_back(p.mLocation);
  });
  
  return positions;
}

const float PARTICLE_NEARBY_THRESHOLD = 10.f;

std::vector<Particle> WakaFlockingFlameApp::getNearbyParticles(const Particle& p1) const {
  std::vector<Particle> particles;
  
  std::for_each(mParticles.begin(), mParticles.end(), [&p1, &particles] (const Particle& p2) {
    auto dis = dot(p1.mLocation, p2.mLocation);
    if (dis > 0 && dis < PARTICLE_NEARBY_THRESHOLD) {
      particles.push_back(p2);
    }
  });
  
  return particles;
}

void WakaFlockingFlameApp::setup()
{
  gl::enableDepthRead();
  gl::enableDepthWrite();
  
  mCamera.lookAt(glm::vec3{3,2,-10}, glm::vec3{0});
  
  int numParticles = 100;
  auto rand = ci::Rand{};
  
  for (int i = 0; i < numParticles; ++i) {
    auto x = rand.nextFloat();
    auto y = rand.nextFloat();
    auto z = rand.nextFloat();
    mParticles.push_back(Particle{glm::vec3{x,y,z}});
  }
}

void WakaFlockingFlameApp::mouseDown( MouseEvent event )
{
}

void WakaFlockingFlameApp::update()
{
  std::for_each(mParticles.begin(), mParticles.end(), [this] (Particle& p) {
    auto nearby = getNearbyParticles(p);
    p.update();
  });
}

void WakaFlockingFlameApp::draw()
{
  gl::clear( Color( 0, 0, 0 ) );
  gl::setMatrices(mCamera);
  auto lambert = gl::ShaderDef{}.lambert();
  auto shader = gl::getStockShader(lambert);
  shader->bind();
  std::for_each(mParticles.begin(), mParticles.end(), [] (Particle& p) { p.draw(); });
}

CINDER_APP( WakaFlockingFlameApp, RendererGl )
