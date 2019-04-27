class Pelicula {

	//1) Constructor
	constructor(i, t, d, e, p, v){
		this.ID = i
		this.Titulo = t
		this.Descripcion = d
		this.Estreno = e
		this.Poster = (p != null) ? p : "http://www.seagarden.cl/home/images/contenido-no-disponible.jpg" 
		this.Trailer = v
	}

	//2) Metodos de Instancia
	Mostrar(){
		document.write(`
			<img src="${this.Poster}" width="150">
			<h1>${this.Titulo} (${this.Estreno})</h1>
			<p>${this.Descripcion}</p>
			<a href="${this.Trailer}" target="_blank">Reproducir Trailer</a>
			<hr>
		`)
	}

	Imprimir(area){

		let ficha = document.querySelector(".pelicula").cloneNode(true)

		ficha.querySelector("h4").innerText = this.Titulo
		ficha.querySelector("p").innerText = this.Estreno
		ficha.querySelector("img").src = this.Poster

		ficha.querySelector("a").onclick = () => {

			//window.alert(`Usted quiere ver: ${this.Trailer}`)
			document.querySelector("#playMovie iframe").src = `https://youtube.com/embed/${this.Trailer}`

		}

		ficha.classList.remove("hide")

		console.log(ficha)

		document.querySelector(area).appendChild(ficha)
	}

	render(DOM){ //<-- #peliculas
		let vDOM = document.createElement("div") //<-- <div></div>
		vDOM.classList.add("col-md-3", "col-sm-4", "pelicula")
		vDOM.id = `movie-${this.ID}`
		vDOM.innerHTML = `<div class="thumbnail">
							<img src="${this.Poster}" alt="">
								<div class="caption">
									<h4>${this.Titulo}</h4>
									<p>${this.Estreno}</p>
									<p><a href="#playMovie" class="btn btn-primary">Ver ahora</a></p>
								</div>
						  </div>`
		vDOM.querySelector("a").onclick = event => {
			event.preventDefault()
			Reproductor.play(this)
		}
		
		document.querySelector(DOM).appendChild(vDOM)
	}

	//3) Metodos de Clase (ó Método Estático)
	static parse(json){
		let data = JSON.parse( json )

		if( data instanceof Array ){
			/*
			let peliculas = new Array()
			data.forEach(function(pelicula){
				peliculas.push( new Pelicula(pelicula.idPelicula, pelicula.Titulo, pelicula.Descripcion, pelicula.Estreno, pelicula.Poster, pelicula.Trailer) )
			})
			return peliculas;
			*/

			return data.map( pelicula => new Pelicula(pelicula.idPelicula, pelicula.Titulo, pelicula.Descripcion, pelicula.Estreno, pelicula.Poster, pelicula.Trailer) )

		} else if( data instanceof Object ){

			return new Pelicula(data.idPelicula, data.Titulo, data.Descripcion, data.Estreno, data.Poster, data.Trailer)

		} else {
			throw "Error: el formato no puede usarse para crear objetos Pelicula"
		}
	}
}

class Reproductor extends Pelicula {

	static play(pelicula){
		let vDOM = document.querySelector("#playMovie")
		    vDOM.querySelector("#titulo").innerText = `${pelicula.Titulo} (${pelicula.Estreno})`
		    vDOM.querySelector("iframe").src = pelicula.Trailer
		    vDOM.querySelector("#descripcion").innerText = pelicula.Descripcion
		    vDOM.querySelector("#imagen").src = pelicula.Poster

		    window.scroll({
		    	top : vDOM.offsetTop,
		    	behavior : "smooth"
		    })

	}

}