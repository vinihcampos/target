#include "SampleIntegrator.h"
#include "Ray.h"
#include "PPM.h"

void target::SampleIntegrator::render( const Scene& scene ){
    preprocess(scene);

    float counter = 0;
    int max_size = camera.get()->buffer->getHeight() * camera.get()->buffer->getWidth();
    for ( int row = 0 ; row < camera.get()->buffer->getHeight(); row++ ) {
        for( int col = 0 ; col < camera.get()->buffer->getWidth() ; col++ ) {
			Ray ray = camera.get()->generate_ray( row, col ); 
            Color L = (Li( ray, scene, *sampler.get(), 1) * 255).clamp(0.f, 255.f);
            camera.get()->buffer->pixel( Point2(col, row), L );
            std::cout << "Loading...[" << int(counter / max_size * 100) << "%]\r";
            std::fflush ( stdin );
			counter += 1;
        }
    }
    std::cout << "Loading...[100%]" << std::endl;
    PPM::generator(camera.get()->buffer, name);
}