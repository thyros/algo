fn row_weights(array: Vec<u32>) -> (u32, u32) {
    array.chunks(2).fold((0,0), |acc, xy| {
        (acc.0 + xy[0], acc.1 + xy.get(1).unwrap_or(&0))
    })
}