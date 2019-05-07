#include "SampleIntegrator.h"
#include "Ray.h"
#include "PPM.h"

void target::SampleIntegrator::render( const Scene& scene ){
    preprocess(scene);

    for ( int row = 0 ; row < camera.get()->buffer->getHeight(); row++ ) {
        for( int col = 0 ; col < camera.get()->buffer->getWidth() ; col++ ) {
            Ray ray = camera.get()->generate_ray( row, col ); 
            Color L = Li( ray, scene, row, col, *sampler.get() );
            camera.get()->buffer->pixel( Point2(col, row), L );
        }
    }
    PPM::generator(camera.get()->buffer, name);
}