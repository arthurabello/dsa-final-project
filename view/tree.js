export default function(json, debounce) {
    const r = 800 / 2
    
    const tree = d3.tree()
        .size([360, r - 50])
        .separation((a, b) => (a.parent == b.parent ? 1 : 2) / a.depth)
    
    const line = d3.linkRadial()
        .angle(d => d.x / 180 * Math.PI)
        .radius(d => d.y)
           
    const svg = d3.select("#tree").append("svg")
        .attr("width", r * 2)
        .attr("height", r * 2 - 100)
    
    const g = svg.append("g")

    let maxWeight = 0
    const add_children = function(node, id) {
        for (let c of json.filter(e => e[2] == id)) {
            const n = {
                name: c[0],
                weight: c[3],
                children: []
            }
            maxWeight = Math.max(maxWeight, c[3])
            node.children.push(n)
            add_children(n, c[1])
        }
    }
    const root = json.find(e => e[2] == 0)
    const root_node = {
        name: root[0],
        weight: root[3],
        children: []
    }
    add_children(root_node, root[1])
    
    const color = d3.scaleLinear()
        .domain([0, maxWeight])
        .range(["lightskyblue", "tomato"])
    
    const obj = d3.hierarchy(root_node)
    
    const nodes = tree(obj)
    
    const link = g.selectAll("path.link")
        .data(nodes.links())
        .enter().append("path")
        .attr("class", "link")
        .attr("stroke-width", 2)
        .attr("d", line)
    
    const node = g.selectAll("node")
        .data(nodes)
        .enter()
    
    node.append("circle")
        .attr("r", 4.5)
        .attr("fill", d => color(d.data.weight))
        .attr("transform", d => "rotate(" + (d.x - 90) + ")translate(" + d.y + ")")
    
    node.append("text")
        .attr("dx", d => d.x < 180 ? 8 : -8)
        .attr("dy", ".31em")
        .attr("visibility", "hidden")
        .attr("text-anchor", d => d.x < 180 ? "start" : "end")
        // .attr("transform", d => `rotate(${d.x < 180? 0 : 180})`)
        .attr("transform", d =>
            d.x < 180?
            `rotate(${d.x - 90}) translate(${d.y - 6})` :
            `rotate(${d.x + 90}) translate(${-d.y + 6})`)
        .text(d => d.data.name)
    
    let lastK = 0
    const updateScale = debounce(() => {
        const k = lastK
    
        g.selectAll("text")
            .attr("font-size", d => 15 / k)
            .attr("visibility", k > 2? "visible" : "hidden")
        g.selectAll("circle")
            .attr("r", 4.5 / k)
        g.selectAll("path").attr("stroke-width", 2 / k)
    }, 100)
    
    function onZoom(ev) {
        g.attr("transform", ev.transform)
    
        const k = ev.transform.k
        if (k !== lastK) {
            lastK = k
            updateScale()
        }
    }
    
    const zoom = d3.zoom().scaleExtent([1, 15]).on("zoom", onZoom)
    svg.call(zoom)
    svg.call(zoom.transform, d3.zoomIdentity.translate(r, r - 75))
}
