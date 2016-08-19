/**
 * Shared code module for excess functions that constants that do not belong in any classes defined by the proejct.
 * @author Anthony Agnone, Aug 2016
 *
 * TODO this simply shouldn't exist.
 */

#ifndef OPENGL_SPECTROGRAM_SHARED_HPP
#define OPENGL_SPECTROGRAM_SHARED_HPP

#include <stdexcept>

/**
 * Fully-implemented modulo function to handle a positive or negative number.
 * https://en.wikipedia.org/wiki/Modulo_operation
 * @param dividend the dividend of the operation.
 * @param divisor the divisor of the operation.
 * @return The remainder of the Euclidean division of dividend by divisor.
 */
int mod(int dividend, int divisor);

#endif /* OPENGL_SPECTROGRAM_SHARED_HPP */
